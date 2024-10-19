/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_STATUS_REPOSITORY_H
#define TKS_STATUS_SERVER_STATUS_REPOSITORY_H

#include <vector>
#include "application/domain/ActiveSession.h"

struct StatusRepository {

    virtual void save_online_session_ttl(ActiveSession const &session, int32_t ttl_sec) = 0;

    virtual void delete_user_session(int32_t user_id, int64_t auth_key_id) = 0;

    virtual std::vector<std::string> get_user_online_sessions(int32_t user_id) = 0;
};


#endif //TKS_STATUS_SERVER_STATUS_REPOSITORY_H
