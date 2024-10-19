/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_ACTIVE_SESSION_H
#define TKS_STATUS_SERVER_ACTIVE_SESSION_H

#include <sstream>
#include <string>
#include <cinttypes>

struct ActiveSession {
    int64_t auth_key_id;
    int32_t server_id;
    int32_t user_id;
    int32_t app_version;

    [[nodiscard]] std::string as_string() const {
        std::stringstream ss;
        ss << auth_key_id << "_" << server_id << "_" << user_id << "_" << app_version;
        return ss.str();
    }
};


#endif //TKS_STATUS_SERVER_ACTIVE_SESSION_H
