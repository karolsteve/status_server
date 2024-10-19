/*
 * @author: Steve Tchatchouang
 * Date : 19/10/24
 */

#ifndef TKS_STATUS_SERVER_STATUS_SERVICE_IMPL_H
#define TKS_STATUS_SERVER_STATUS_SERVICE_IMPL_H

#include "application/port/in/StatusService.h"
#include "application/port/out/persistence/StatusRepository.h"

class StatusServiceImpl : public StatusService {
private:
    StatusRepository *p_repository;

public:
    explicit StatusServiceImpl(StatusRepository *status_repository);

    void set_session_online_ttl(ActiveSession const &session, int32_t ttl_sec) override;

    void set_session_offline(int32_t user_id, int64_t auth_key_id) override;

    std::vector<std::string> get_user_online_sessions(int32_t user_id) override;

    virtual ~StatusServiceImpl() = default;
};


#endif //TKS_STATUS_SERVER_STATUS_SERVICE_IMPL_H
