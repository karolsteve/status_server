/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#include "in/grpc/StatusRPCServerLauncher.h"
#include "StatusGrpcServer.h"

void StatusRPCServerLauncher::launch(const std::string &host_ip, int32_t port, const std::string &rpc_addr,
                                     const std::string &service_name, const std::vector<std::string> &etcd_hosts,
                                     const std::string &node_id, StatusService *p_status_service) {

    StatusGrpcServer server{p_status_service};
    server.run(host_ip, port, rpc_addr, service_name, etcd_hosts, node_id);
}
