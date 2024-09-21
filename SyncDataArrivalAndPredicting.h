#pragma once

//#include "Header.h"
#include <mutex>
#include <condition_variable>

//extern // xxxx

class SyncDataArrivalAndPredicting {
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

    SyncDataArrivalAndPredicting() {firstMsgArrived = false;}

    void WaitForFirstMsg();

    void FirstMsgArrived();
};