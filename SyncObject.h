#pragma once
#include <mutex>
#include <condition_variable>

class SyncObject {
    public:
    bool firstMsgArrived_;

    std::condition_variable cvFirstMsg_;
    std::mutex mtxFirstMsg_;

    int              condition_predicate_color_ = 0; // xxxx needed?
    pthread_mutex_t  condition_lock_color_;
    pthread_cond_t   condition_variable_color_;

    int              condition_predicate_finished_ = 0; // xxxx needed?
    pthread_mutex_t  condition_lock_finished_;
    pthread_cond_t   condition_variable_finished_;

    std::mutex syncDetectSetBITA_mutex_;
    std::condition_variable syncDetectSetBITA_cv_;
    bool syncDetectSetBITA_ready_ = false;

    bool FINISHED = false;
    

    SyncObject() {
        pthread_mutex_init(&this->condition_lock_color_, NULL);
        pthread_cond_init(&this->condition_variable_color_, NULL);
        pthread_mutex_init(&this->condition_lock_finished_, NULL);
        pthread_cond_init(&this->condition_variable_finished_, NULL);
        firstMsgArrived_ = false;
    }

    void WaitForFirstMsg();

    void FirstMsgArrived();
};