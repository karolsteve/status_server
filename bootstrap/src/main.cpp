#include <iostream>
#include <yaml-cpp/yaml.h>
#include "ConfigLoader.h"

#include <memory>
#include "adapter/out/persistence/redis/RedisStatusRepository.h"
#include "application/service/StatusServiceImpl.h"
#include "adapter/in/grpc/StatusRPCServerLauncher.h"

static std::string getenv_var(std::string const &name) {
    const char *res = std::getenv(name.c_str());
    return res == nullptr ? std::string{} : std::string{res};
}

static std::string get_config_file_path(int argc, char **argv) {
    std::string config_file_path;

    if (argc > 1) {
        config_file_path = std::string(argv[1]);
    } else {
        config_file_path = getenv_var("STATUS_CONFIG_PATH");
    }
    return config_file_path;
}

int main(int argc, char **argv) {
    std::string config_file_path = get_config_file_path(argc, argv);
    if (config_file_path.empty()) {
        std::cerr << "Can not load config file. pass config as first arg or define STATUS_CONFIG_PATH env var\n";
        return EXIT_FAILURE;
    }

    StatusConfig status_config = ConfigLoader::load_config(config_file_path);
    std::cout << "Config Loaded: \n" << status_config << std::endl;

    ServerConfig serverConfig = status_config.server;

    //repo
    auto status_repository = std::make_unique<RedisStatusRepository>(serverConfig.redis.hosts[0]);

    //services
    auto status_service = std::make_unique<StatusServiceImpl>(status_repository.get());

    //server
    StatusRPCServerLauncher::launch(
            status_config.ext_ip,
            std::stoi(serverConfig.addr.substr(serverConfig.addr.find(':') + 1)),
            serverConfig.addr,
            serverConfig.discovery.service_name,
            serverConfig.discovery.etcd.hosts,
            "node" + std::to_string(status_config.server_id),
            status_service.get());

    return EXIT_SUCCESS;
}
