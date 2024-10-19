//
// Created by Steve Tchatchouang on 28/09/23.
//

#include "EtcdWatcher.h"
#include "fastlog/FastLog.h"

#include <utility>

EtcdWatcher *EtcdWatcher::make(const std::string &client_url, const std::string &service_name, EtcdWatcherDelegate *d) {
    auto *result = new EtcdWatcher(client_url, service_name);
    result->delegate = d;
    return result;
}

EtcdWatcher::EtcdWatcher(const std::string &client_url, std::string service_name) :
        m_client(std::unique_ptr<etcd::Client>(etcd::Client::WithUrl(client_url))),
        m_service_name(std::move(service_name)) {

}

void EtcdWatcher::start(std::string const &service_dir) {
    watch_keys(service_dir);
    resolve(service_dir);
}

void EtcdWatcher::stop() {
    m_watcher->Cancel();
}

void EtcdWatcher::watch_keys(std::string const &service_dir) {
    wait_for_connection();
    if (m_watcher && m_watcher->Cancelled()) {
        std::cout << "watcher's reconnect loop been cancelled\n";
        return;
    }

    std::string root_path = service_dir + "/" + m_service_name + "/";
    std::cout << "Watching " << root_path << " sn is " << m_service_name << " ...\n";

    m_watcher = std::make_unique<etcd::Watcher>(*m_client, root_path, [this](const etcd::Response &response) {
        DEBUG_D("EVENT OCCUR srv %s. action %s. key %s. value %s", m_service_name.c_str(),
                response.action().c_str(), response.value().key().c_str(), response.value().as_string().c_str());

        if (response.action() == "set" || response.action() == "launch") {
            if(add_node(response.value().key(), response.value().as_string())){
                delegate->on_nodes_updated();
            }
        } else if (response.action() == "delete") {
            std::string key = response.value().key();
            if(remove_node(key)){
                delegate->on_nodes_updated();
            }
        }
    }, true);

    m_watcher->Wait([this, service_dir](bool cancelled) {
        if (cancelled) {
            std::cout << "watcher's reconnect loop stopped as been cancelled" << std::endl;
            return;
        }
        watch_keys(service_dir);
    });
}

void EtcdWatcher::resolve(std::string const &service_dir) {
    std::string path = service_dir + "/" + m_service_name + "/";
    m_client->ls(path).then([this](pplx::task<etcd::Response> const &response_task) {
        const etcd::Response response = response_task.get();
        bool changed{false};
        for (int i = 0; i < (int32_t) response.values().size(); ++i) {
            std::string k = response.value(i).key();
            std::string v = response.value(i).as_string();
            bool r = add_node(k, v);
            if(!changed){
                changed = r;
            }
        }

        if(changed){
            delegate->on_nodes_updated();
        }
    });
}

bool EtcdWatcher::add_node(const std::string &key, const std::string &value) {
    std::unique_lock write_lock(mutex);
    if (m_nodes.find(key) != m_nodes.end()) {
        if (m_nodes[key]->addr != value) {
            std::string old = m_nodes[key]->addr;

            auto *data = new NodeData;
            data->addr = value;
            data->node_id = std::stoi(key.substr(key.find_last_of('/') + 5));
            m_nodes[key] = std::unique_ptr<NodeData>(data);
            return true;
        }
    } else {
        auto *data = new NodeData;
        data->addr = value;
        data->node_id = std::stoi(key.substr(key.find_last_of('/') + 5));
        m_nodes[key] = std::unique_ptr<NodeData>(data);
        return true;
    }
    return false;
}

bool EtcdWatcher::remove_node(const std::string &key) {
    std::unique_lock write_lock(mutex);
    auto const iterator = m_nodes.find(key);
    if (iterator != m_nodes.end()) {
        m_nodes.erase(iterator);
        return true;
    }
    return false;
}

std::unordered_map<std::string, std::shared_ptr<EtcdWatcher::NodeData>> EtcdWatcher::updated_nodes() {
    std::shared_lock read_lock(mutex);
    return m_nodes;
}

void EtcdWatcher::wait_for_connection() {
    while (!m_client->head().get().is_ok()) {
        sleep(1);
    }
}