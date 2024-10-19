//
// Created by steve on 13/10/23.
//

#ifndef TKS_STATUS_SERVER_TIMER_TASK_H
#define TKS_STATUS_SERVER_TIMER_TASK_H

#include <functional>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>

class TimerTask {
private:

    int64_t m_duration;
    std::function<void()> m_func;
    bool m_auto_repeat{false};
    bool m_running{false};
    bool m_start_immediately{false};
    std::mutex m_mutex{};
    std::condition_variable m_cond{};
    std::thread m_thread;

public:
    explicit TimerTask(int64_t duration, std::function<void()> func);
    ~TimerTask();

    inline void set_auto_repeat(bool auto_repeat){
        assert(!m_running);
        m_auto_repeat = auto_repeat;
    }

    inline void set_should_start_immediately(bool start){
        assert(!m_running);
        m_start_immediately = start;
    }

    void start();
    void stop();
};


#endif //TKS_STATUS_SERVER_TIMER_TASK_H
