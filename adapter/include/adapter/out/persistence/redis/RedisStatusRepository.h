/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_REDIS_STATUS_REPOSITORY_H
#define TKS_STATUS_SERVER_REDIS_STATUS_REPOSITORY_H

#include <memory>
#include <string>
#include <sw/redis++/redis++.h>
#include "application/port/out/persistence/StatusRepository.h"

class RedisStatusRepository : public StatusRepository {
private:
    std::unique_ptr<sw::redis::Redis> m_redis{nullptr};
public:
    explicit RedisStatusRepository(std::string const &redis_url);

    void save_online_session_ttl(const ActiveSession &session, int32_t ttl_sec) override;

    void delete_user_session(int32_t user_id, int64_t auth_key_id) override;

    std::vector<std::string> get_user_online_sessions(int32_t user_id) override;

    virtual ~RedisStatusRepository() = default;
};


#endif //TKS_STATUS_SERVER_REDIS_STATUS_REPOSITORY_H
