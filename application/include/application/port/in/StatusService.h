/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_STATUS_SERVICE_H
#define TKS_STATUS_SERVER_STATUS_SERVICE_H

#include <string>
#include <vector>
#include "application/domain/ActiveSession.h"

struct StatusService {
    virtual void set_session_online_ttl(ActiveSession const &session, int32_t ttl_sec) = 0;

    virtual void set_session_offline(int32_t user_id, int64_t auth_key_id) = 0;

    virtual std::vector<std::string> get_user_online_sessions(int32_t user_id) = 0;
};


#endif //TKS_STATUS_SERVER_STATUS_SERVICE_H
