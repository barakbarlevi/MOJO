#include "SyncObject.h"

void SyncObject::WaitForFirstMsg() {
        std::unique_lock<std::mutex> lk(mtxFirstMsg);
        cvFirstMsg.wait(lk, [this]{ 
        return firstMsgArrived == true; 
        });
    }

void SyncObject::FirstMsgArrived() {
    {
        std::lock_guard<std::mutex> lk(mtxFirstMsg);
        firstMsgArrived = true;
    }
    cvFirstMsg.notify_one(); // let the waiter know
}