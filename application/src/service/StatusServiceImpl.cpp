/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#include "service/StatusServiceImpl.h"

StatusServiceImpl::StatusServiceImpl(StatusRepository *status_repository) : p_repository(status_repository) {}

void StatusServiceImpl::set_session_online_ttl(ActiveSession const &session, int32_t ttl_sec) {
    p_repository->save_online_session_ttl(session, ttl_sec);
}

void StatusServiceImpl::set_session_offline(int32_t user_id, int64_t auth_key_id) {
    p_repository->delete_user_session(user_id, auth_key_id);
}

std::vector<std::string> StatusServiceImpl::get_user_online_sessions(int32_t user_id) {
    return p_repository->get_user_online_sessions(user_id);
}
