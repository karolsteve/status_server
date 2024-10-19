/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#include "out/persistence/redis/RedisStatusRepository.h"

static const std::string USER_KEY_ID_PREFIX = "user_online_keys";

static std::string get_user_key(int32_t user_id) {
    return USER_KEY_ID_PREFIX + "#" + std::to_string(user_id);
}

RedisStatusRepository::RedisStatusRepository(const std::string &redis_url) {
    size_t idx = redis_url.find_last_of(':');
    std::string host = redis_url.substr(0, idx);
    std::string port = redis_url.substr(idx + 1);

    sw::redis::ConnectionOptions connection_options{};
    connection_options.host = host.substr(host.find_last_of('/') + 1);
    connection_options.port = std::stoi(port);

    sw::redis::ConnectionPoolOptions pool_options{};
    pool_options.size = 8;

    m_redis = std::make_unique<sw::redis::Redis>(connection_options, pool_options);
}

void RedisStatusRepository::save_online_session_ttl(const ActiveSession &session, int32_t ttl_sec) {
    std::string key = get_user_key(session.user_id);

    m_redis->hset(key, std::to_string(session.auth_key_id), session.as_string());
    m_redis->expire(key, ttl_sec);
}

void RedisStatusRepository::delete_user_session(int32_t user_id, int64_t auth_key_id) {
    std::string key = get_user_key(user_id);

    m_redis->hdel(key, std::to_string(auth_key_id));
}

std::vector<std::string> RedisStatusRepository::get_user_online_sessions(int32_t user_id) {
    std::string key = get_user_key(user_id);

    std::unordered_map<std::string, std::string> m{};
    m_redis->hgetall(key, std::inserter(m, m.begin()));

    std::vector<std::string> result{};

    for (const auto &item: m) {
        result.push_back(item.second);
    }

    return result;
}
