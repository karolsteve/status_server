//
// Created by steve on 13/10/23.
//

#include "TimerTask.h"

TimerTask::TimerTask(int64_t duration, std::function<void()> func): m_duration(duration), m_func(std::move(func)) {

}

void TimerTask::start() {
    assert(!m_running);
    m_running = true;
    m_thread = std::thread([this]{
        printf("Thread started\n");
        if(m_start_immediately){
            m_func();
            if(!m_auto_repeat){
                return;
            }
        }

        while (m_running){
            {
                std::unique_lock lock(m_mutex);
                if(m_cond.wait_for(lock, std::chrono::milliseconds (m_duration)) == std::cv_status::no_timeout){
                    abort();
                }
            }

            if(m_running){
                m_func();
                if(!m_auto_repeat){
                    break;
                }
            }
        }
    });
}

void TimerTask::stop() {
    printf("Stop called A\n");
    if(!m_running){
        return;
    }
    printf("Stop called B\n");

    m_running = false;
    m_cond.notify_one();
    m_thread.join();
}

TimerTask::~TimerTask() {
    stop();
}
