/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_STATUS_RPC_SERVER_LAUNCHER_H
#define TKS_STATUS_SERVER_STATUS_RPC_SERVER_LAUNCHER_H

#include <string>
#include <vector>
#include "application/port/in/StatusService.h"

class StatusRPCServerLauncher {
public:
    /**
     * Create StatusRPCServer, run and block until end
     * @param host_ip public host ip address
     * @param port port
     * @param rpc_addr rpc address
     * @param service_name service name for registry
     * @param etcd_hosts etcd cluster nodes
     * @param node_id node id for service registry
     */
    static void launch(const std::string &host_ip, int32_t port, std::string const &rpc_addr,
                       const std::string &service_name, const std::vector<std::string> &etcd_hosts,
                       const std::string &node_id, StatusService *p_status_service);
};


#endif //TKS_STATUS_SERVER_STATUS_RPC_SERVER_LAUNCHER_H
