#include "SyncObject.h"

void SyncObject::WaitForFirstMsg() {
        std::unique_lock<std::mutex> lk(mtxFirstMsg_);
        cvFirstMsg_.wait(lk, [this]{ 
        return firstMsgArrived_ == true; 
        });
    }

void SyncObject::FirstMsgArrived() {
    {
        std::lock_guard<std::mutex> lk(mtxFirstMsg_);
        firstMsgArrived_ = true;
    }
    cvFirstMsg_.notify_one();
}