//
// Created by Steve Tchatchouang on 28/09/23.
//

#ifndef TKS_GATEWAY_SERVER_ETCD_WATCHER_H
#define TKS_GATEWAY_SERVER_ETCD_WATCHER_H

#include <etcd/Client.hpp>
#include <etcd/Watcher.hpp>
#include <string>
#include <memory>
#include <vector>
#include <shared_mutex>
#include <mutex>

typedef struct EtcdWatcherDelegate {
    virtual void on_nodes_updated() = 0;
} EtcdWatcherDelegate;

class EtcdWatcher {
public:
    typedef struct NodeData {
        std::string addr;
        int32_t node_id;

        bool operator==(NodeData const &rhs) const {
            return this->addr == rhs.addr && this->node_id == rhs.node_id;
        }
    } NodeData;

public:
    static EtcdWatcher *make(std::string const &client_url, std::string const &service_name, EtcdWatcherDelegate *d);

private:
    std::unique_ptr<etcd::Client> m_client;
    std::string m_service_name;
    EtcdWatcherDelegate *delegate{nullptr};
    std::unique_ptr<etcd::Watcher> m_watcher{nullptr};
    std::unordered_map<std::string, std::shared_ptr<NodeData>> m_nodes{};
    std::shared_mutex mutex{};

private:
    EtcdWatcher(std::string const &client_url, std::string service_name);

    void watch_keys(std::string const &service_dir);

    void resolve(std::string const &service_dir);

    void wait_for_connection();

    bool add_node(std::string const &k, std::string const &v);

    bool remove_node(std::string const &k);

public:
    void start(std::string const &service_dir);

    void stop();

    std::unordered_map<std::string, std::shared_ptr<EtcdWatcher::NodeData>> updated_nodes();
};


#endif //TKS_GATEWAY_SERVER_ETCD_WATCHER_H
