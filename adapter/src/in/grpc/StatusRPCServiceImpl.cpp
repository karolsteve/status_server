//
// Created by Steve Tchatchouang on 13/10/23.
//

#include "StatusRPCServiceImpl.h"
#include "fastlog/FastLog.h"
#include <sstream>
#include <unordered_map>
#include "application/domain/ActiveSession.h"

StatusRPCServiceImpl::StatusRPCServiceImpl(StatusService *status_service) : p_status_service(status_service) {}

::grpc::Status StatusRPCServiceImpl::setSessionOnlineTTL(::grpc::ServerContext *, const ::SetStatusOnlineReq *request,
                                                         ::SetStatusVoidResp *) {

    DEBUG_D("Set online ttl called for %ld, %d, %d", request->authkeyid(), request->userid(), request->ttlsec());

    try {
        int64_t auth_key_id = request->authkeyid();
        int32_t server_id = request->serverid();
        int32_t user_id = request->userid();
        int32_t app_version = request->appversion();
        int32_t ttl_sec = request->ttlsec();

        struct ActiveSession as{auth_key_id, server_id, user_id, app_version};

        p_status_service->set_session_online_ttl(as, ttl_sec);

        return grpc::Status::OK;
    } catch (std::exception const &e) {
        DEBUG_F("Got error on set sess online ttl: %s", e.what());
        return {grpc::StatusCode::UNKNOWN, e.what()};
    }
}

::grpc::Status StatusRPCServiceImpl::setSessionOffline(::grpc::ServerContext *, const ::SetStatusOfflineReq *request,
                                                       ::SetStatusVoidResp *) {

    DEBUG_D("Set offline called for %ld, %d", request->authkeyid(), request->userid());

    try {
        int64_t auth_key_id = request->authkeyid();
        int32_t user_id = request->userid();

        p_status_service->set_session_offline(user_id, auth_key_id);

        return grpc::Status::OK;
    } catch (std::exception const &e) {
        DEBUG_F("Got error on set sess online ttl: %s", e.what());
        return {grpc::StatusCode::UNKNOWN, e.what()};
    }
}

::grpc::Status StatusRPCServiceImpl::getUserOnlineSessions(::grpc::ServerContext *, const ::StatusReqUserID *request,
                                                           ::UserOnlineSessions *response) {

    DEBUG_D("get user online sessions called for  %d", request->userid());

    try {
        int32_t user_id = request->userid();

        std::vector<std::string> result = p_status_service->get_user_online_sessions(user_id);

        for (const auto &item: result) {
            response->add_activesession(item);
        }

        return grpc::Status::OK;
    } catch (std::exception const &e) {
        DEBUG_F("Got error on set sess online ttl: %s", e.what());
        return {grpc::StatusCode::UNKNOWN, e.what()};
    }
}
