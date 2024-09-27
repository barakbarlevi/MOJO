#pragma once
#include <mutex>
#include <condition_variable>

class SyncObject {
    public:
    bool firstMsgArrived;

    std::condition_variable cvFirstMsg;
    std::mutex mtxFirstMsg;

    int              condition_predicate_color = 0; // xxxx needed?
    pthread_mutex_t  condition_lock_color;
    pthread_cond_t   condition_variable_color;

    int              condition_predicate_finished = 0; // xxxx needed?
    pthread_mutex_t  condition_lock_finished;
    pthread_cond_t   condition_variable_finished;

    std::mutex syncDetectSetBITA_mutex;
    std::condition_variable syncDetectSetBITA_cv;
    bool syncDetectSetBITA_ready = false;
    

    SyncObject() {
        pthread_mutex_init(&this->condition_lock_color, NULL);
        pthread_cond_init(&this->condition_variable_color, NULL);
        pthread_mutex_init(&this->condition_lock_finished, NULL);
        pthread_cond_init(&this->condition_variable_finished, NULL);
        firstMsgArrived = false;
    }

    void WaitForFirstMsg();

    void FirstMsgArrived();
};