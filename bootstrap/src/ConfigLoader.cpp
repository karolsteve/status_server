/*
 * @author: Steve Tchatchouang
 * Date : 12/10/24
 */
//

#include "ConfigLoader.h"
#include <yaml-cpp/yaml.h>
#include <thread>

StatusConfig ConfigLoader::load_config(const std::string& config_file_path) {
    std::cout << "Loading config from file path: " << config_file_path << std::endl;

    const YAML::Node config = YAML::LoadFile(config_file_path);

    StatusConfig status_config{};
    status_config.debug_mode = config["DebugMode"].as<int32_t>();
    status_config.log_enabled = config["LogEnabled"].as<int32_t>();
    status_config.log_verbose = config["LogVerbose"].as<int32_t>();
    status_config.server_id = config["ServerID"].as<int32_t>();
    status_config.ext_ip = config["PublicIP"].as<std::string>();

    //root
    YAML::Node config_root = config["Status"];

    //rpc server
    {
        YAML::Node server = config_root["Server"];
        auto name = server["Name"].as<std::string>();
        auto addr = server["Addr"].as<std::string>();

        YAML::Node discovery = server["Discovery"];
        auto service_name = discovery["ServiceName"].as<std::string>();

        struct EtcdConfig etcd_config{};
        //Etcd
        {
            YAML::Node etcd_root = discovery["Etcd"];
            auto etcd_hosts = etcd_root["Hosts"];
            for (YAML::const_iterator it = etcd_hosts.begin(); it != etcd_hosts.end(); it++) {
                etcd_config.hosts.push_back(it->as<std::string>());
            }
        }

        struct RedisConfig redis_config{};
        //Redis
        {
            YAML::Node redis_root = server["Redis"];
            auto redis_hosts = redis_root["Hosts"];
            for (YAML::const_iterator it = redis_hosts.begin(); it != redis_hosts.end(); it++) {
                redis_config.hosts.push_back(it->as<std::string>());
            }
        }

        status_config.server = {name, addr, {service_name, etcd_config}, redis_config};
    }

    return status_config;
}
