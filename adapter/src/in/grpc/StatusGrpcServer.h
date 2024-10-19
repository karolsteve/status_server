/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_STATUS_GRPC_SERVER_H
#define TKS_STATUS_SERVER_STATUS_GRPC_SERVER_H

#include <string>
#include <vector>
#include "application/port/in/StatusService.h"
#include "StatusRPCServiceImpl.h"
#include "etcdregistry/EtcdRegistry.h"

class StatusGrpcServer final {
private:
    StatusService *p_status_service;
    std::unique_ptr<grpc::Server> m_server{nullptr};
    StatusRPCServiceImpl *m_service_impl{nullptr};
    std::unique_ptr<EtcdRegistry> m_registry{nullptr};

private:
    void setup_and_start(std::string const &host_ip, int32_t port, std::string const &rpc_addr,
                         std::string const &service_name, std::vector<std::string> const &etcd_hosts,
                         std::string const &node_id);

    [[nodiscard]] static EtcdRegistry *
    setup_registry(std::string const &ip, int32_t port, std::string const &service_name,
                   std::vector<std::string> const &etcd_hosts,
                   std::string const &node_id);

    void shutdown();

    void block_until_finish();

public:
    explicit StatusGrpcServer(StatusService *status_service);

    virtual ~StatusGrpcServer() final;

    void run(std::string const &host_ip, int32_t port, std::string const &rpc_addr, std::string const &service_name,
             std::vector<std::string> const &etcd_hosts, std::string const &node_id);
};


#endif //TKS_STATUS_SERVER_STATUS_GRPC_SERVER_H
