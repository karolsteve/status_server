//
// Created by Steve Tchatchouang on 17/04/23.
//

#ifndef TKS_GATEWAY_SERVER_ETCD_REGISTRY_H
#define TKS_GATEWAY_SERVER_ETCD_REGISTRY_H

#include <vector>
#include <string>
#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include "TimerTask.h"

class EtcdRegistry {
public:
    struct Config {
        std::vector<std::string> end_points;
        std::string service_dir;
        std::string service_name;
        std::string node_id;
        std::string value;
        uint32_t ttl;
    };

    static EtcdRegistry *create(Config config);

    void register_();

    void unregister_();

    ~EtcdRegistry();

private:
    std::unique_ptr<etcd::Client> m_client;
    std::string m_key;
    std::string m_value;
    uint32_t m_ttl;
    TimerTask *m_timer_task{nullptr};
    std::shared_ptr<etcd::KeepAlive> m_keep_alive{nullptr};
    int64_t m_lease_id{0};
private:
    EtcdRegistry(const std::string& url, std::string key, std::string value, uint32_t ttl);
    void timer_func();
    void wait_for_connection();
};


#endif //TKS_GATEWAY_SERVER_ETCD_REGISTRY_H
