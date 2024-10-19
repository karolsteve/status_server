//
// Created by steve Tchatchouang on 17/04/23.
//

#ifndef TKS_STATUS_SERVER_STATUS_CONFIG_H
#define TKS_STATUS_SERVER_STATUS_CONFIG_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

struct EtcdConfig {
    std::vector<std::string> hosts;

    friend std::ostream &operator<<(std::ostream &out, const EtcdConfig &config) {
        for (const auto &item: config.hosts) {
            out << item << std::endl;
        }
        return out;
    }
};

struct RedisConfig {
    std::vector<std::string> hosts;

    friend std::ostream &operator<<(std::ostream &out, const RedisConfig &config) {
        for (const auto &item: config.hosts) {
            out << item << std::endl;
        }
        return out;
    }
};

struct Discovery {
    std::string service_name;
    struct EtcdConfig etcd;

    friend std::ostream &operator<<(std::ostream &out, const struct Discovery &config) {
        out << "Service Name : " << config.service_name << std::endl;
        out << "[Discovery -> Etcd]" << std::endl;
        out << config.etcd;
        return out;
    }
};

struct ServerConfig {
    std::string name;
    std::string addr;
    struct Discovery discovery;
    struct RedisConfig redis;

    friend std::ostream &operator<<(std::ostream &out, const ServerConfig &config) {
        out << "Name : " << config.name << " | addr " << config.addr << std::endl;
        out << "[Discovery]" << std::endl;
        out << config.discovery;
        out << "[Redis]" << std::endl;
        out << config.redis;
        return out;
    }
};

typedef struct StatusConfig {
    int32_t debug_mode;
    int32_t log_enabled;
    int32_t log_verbose;
    int32_t server_id;
    std::string ext_ip;
    struct ServerConfig server;

    friend std::ostream &operator<<(std::ostream &out, const StatusConfig &config) {
        out << "DEBUG " << config.debug_mode << "\n";
        out << "LogEnabled " << config.log_enabled << "\n";
        out << "LogVerbose " << config.log_verbose << "\n";
        out << "ServerID " << config.server_id << "\n";
        out << "PublicIP " << config.ext_ip << "\n";
        out << "[Server]" << std::endl;
        out << config.server;
        return out;
    }
} StatusConfig;
#endif //TKS_STATUS_SERVER_STATUS_CONFIG_H
