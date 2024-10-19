//
// Created by Steve Tchatchouang on 17/04/23.
//

#include "EtcdRegistry.h"
#include <iostream>
#include <memory>
#include <utility>
#include "fastlog/FastLog.h"

EtcdRegistry *EtcdRegistry::create(EtcdRegistry::Config config) {
    std::string url{};
    std::for_each(config.end_points.begin(), config.end_points.end(), [&url](std::string &link) {
        if (!url.empty()) {
            url += ",";
        }
        url += link;
    });

    DEBUG_D("Etcd url is %s", url.c_str());
    std::string key = config.service_dir + "/" + config.service_name + "/" + config.node_id;
    return new EtcdRegistry(url, key, config.value, config.ttl);
}

EtcdRegistry::EtcdRegistry(const std::string &url, std::string key, std::string value, uint32_t ttl) :
        m_client(std::unique_ptr<etcd::Client>(etcd::Client::WithUrl(url))), m_key(std::move(key)),
        m_value(std::move(value)),
        m_ttl(ttl) {
    //run every second to check leave renew health
    m_timer_task = new TimerTask(/*(ttl - 2) **/ 1000, [this] { timer_func(); });
    m_timer_task->set_auto_repeat(true);
    m_timer_task->set_should_start_immediately(true);
}

void EtcdRegistry::timer_func() {
    try {
        wait_for_connection();

        bool should_refresh_lease_id = m_lease_id == 0;
        if (m_keep_alive == nullptr) {
            DEBUG_W("Initiating keep alive");
            should_refresh_lease_id = true;
        } else {
            try {
                m_keep_alive->Check();
                DEBUG_D("Lease remaining time >>> lease is %ld. time : %d", m_lease_id,
                        m_client->leasetimetolive(m_lease_id).get().value().ttl());
            } catch (std::exception const &keep_alive_ex) {
                DEBUG_E("Error on keepalive. prepare to refresh lease id. error is %s", keep_alive_ex.what());
                should_refresh_lease_id = true;
            }
        }

        if (should_refresh_lease_id) {
            etcd::Response resp = m_client->leasegrant((int32_t) m_ttl).get();

            if (!resp.is_ok()) {
                DEBUG_W("Timer lease func fail: code %d, msg %s", resp.error_code(), resp.error_message().c_str());
                return;
            }
            m_lease_id = resp.value().lease();
            DEBUG_D("Renew lease id is %ld", m_lease_id);

            resp = m_client->set(m_key, m_value, m_lease_id).get();
            if (!resp.is_ok()) {
                m_lease_id = 0;
                DEBUG_W("Timer set func fail: code %d, msg %s", resp.error_code(), resp.error_message().c_str());
                return;
            } else {
                m_keep_alive = std::make_shared<etcd::KeepAlive>(*m_client, (int32_t) m_ttl, m_lease_id);
                DEBUG_W("Refresh bounded to Registry update %s ttl is %d lease id is %ld", m_value.c_str(),
                        (int32_t) m_ttl, m_lease_id);
            }
        }
    } catch (std::exception const &exception) {
        DEBUG_E("Timer func exception %s", exception.what());
    }
}

void EtcdRegistry::register_() {
    DEBUG_I("REGISTERING %s. val is %s", m_key.c_str(), m_value.c_str());
    if (m_timer_task != nullptr) {
        m_timer_task->start();
    }
}

void EtcdRegistry::wait_for_connection() {
    while (!m_client->head().get().is_ok()) {
        DEBUG_W("WAITING for connection... etcd");
        sleep(1);
    }
}

void EtcdRegistry::unregister_() {
    if (m_timer_task != nullptr) {
        m_timer_task->stop();
    }
    m_client->rm(m_key);
}

EtcdRegistry::~EtcdRegistry() {
    if (m_timer_task != nullptr) {
        delete m_timer_task;
        m_timer_task = nullptr;
    }
    DEBUG_D("DESTRUCTOR REGISTRY CALLED");
}