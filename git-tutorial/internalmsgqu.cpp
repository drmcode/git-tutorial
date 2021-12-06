#include "internalmsgqu.h"

InternalMsgQueue::InternalMsgQueue():mTerminate(false){}

InternalMsgQueue::~InternalMsgQueue(){}

void InternalMsgQueue::msgSend(const InternalMessage& msg)
{
   std::unique_lock<std::mutex> lLock(mMutex);
   mQueue.push(msg);
   std::cout<<"msg.type: "<<msg.type
            <<", msg.data: "<<msg.data
            <<", owns_lock "
            <<lLock.owns_lock()
            <<std::endl;
   mCond.notify_one();
}

InternalMessage InternalMsgQueue::msgReceive()
{
    std::unique_lock<std::mutex> lLock(mMutex);
    while(mQueue.empty()){
        mCond.wait(lLock);
    }

    InternalMessage result = mQueue.front();
    mQueue.pop();
    return result;
}

