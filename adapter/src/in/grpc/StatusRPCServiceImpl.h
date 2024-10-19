//
// Created by Steve Tchatchouang on 13/10/23.
//

#ifndef TKS_STATUS_SERVER_STATUS_RPC_SERVICE_H
#define TKS_STATUS_SERVER_STATUS_RPC_SERVICE_H

#include <grpc++/grpc++.h>
#include "status.grpc.pb.h"
#include "application/port/in/StatusService.h"

class StatusRPCServiceImpl : public StatusRPCServer::Service {
private:
    StatusService *p_status_service;

public:
    explicit StatusRPCServiceImpl(StatusService *status_service);

public:
    ::grpc::Status setSessionOnlineTTL(::grpc::ServerContext *context, const ::SetStatusOnlineReq *request,
                                       ::SetStatusVoidResp *response) override;

    ::grpc::Status setSessionOffline(::grpc::ServerContext *context, const ::SetStatusOfflineReq *request,
                                     ::SetStatusVoidResp *response) override;

    ::grpc::Status getUserOnlineSessions(::grpc::ServerContext *context, const ::StatusReqUserID *request,
                                         ::UserOnlineSessions *response) override;
};


#endif //TKS_STATUS_SERVER_STATUS_RPC_SERVICE_H
