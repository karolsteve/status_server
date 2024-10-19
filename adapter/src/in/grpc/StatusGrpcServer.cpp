/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#include "StatusGrpcServer.h"
#include "fastlog/FastLog.h"
#include "Constants.h"

StatusGrpcServer::StatusGrpcServer(StatusService *status_service) : p_status_service(status_service) {
}

void StatusGrpcServer::setup_and_start(const std::string &host_ip, int32_t port, std::string const &rpc_addr,
                                       const std::string &service_name, const std::vector<std::string> &etcd_hosts,
                                       const std::string &node_id) {

    m_service_impl = new StatusRPCServiceImpl(p_status_service);

    grpc::ServerBuilder builder{};
    builder.AddListeningPort(rpc_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(m_service_impl);

    m_server = builder.BuildAndStart();
    DEBUG_I("Rpc Server listening on %s", rpc_addr.c_str());

    m_registry.reset(setup_registry(host_ip, port, service_name, etcd_hosts, node_id));
    m_registry->register_();
}

EtcdRegistry *
StatusGrpcServer::setup_registry(const std::string &ip, int32_t port, const std::string &service_name,
                                 const std::vector<std::string> &etcd_hosts, const std::string &node_id) {
    EtcdRegistry::Config registry_config{};
    registry_config.service_dir = SERVICE_DIR;
    registry_config.service_name = service_name;
    registry_config.node_id = node_id;
    registry_config.value = ip + ":" + std::to_string(port);
    registry_config.ttl = 10;
    registry_config.end_points = etcd_hosts;
    return EtcdRegistry::create(registry_config);
}

void StatusGrpcServer::shutdown() {
    m_registry->unregister_();
    m_server->Shutdown();
}

void StatusGrpcServer::block_until_finish() {
    m_server->Wait();
}

void StatusGrpcServer::run(const std::string &host_ip, int32_t port, std::string const &rpc_addr,
                           const std::string &service_name, const std::vector<std::string> &etcd_hosts,
                           const std::string &node_id) {

    setup_and_start(host_ip, port, rpc_addr, service_name, etcd_hosts, node_id);
    block_until_finish();
}

StatusGrpcServer::~StatusGrpcServer() {
    shutdown();

    if (m_service_impl != nullptr) {
        delete m_service_impl;
        m_service_impl = nullptr;
    }
}