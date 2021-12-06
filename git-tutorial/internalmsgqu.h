#ifndef _INTERNALMSGQU_H_
#define _INETRNALMSGQU_H_
#include <memory>
#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "commTypes.hpp"


class InternalMsgQueue
{
    public:
    InternalMsgQueue();//construct
    ~InternalMsgQueue();//destruct
    void msgSend(const InternalMessage& msg);
    InternalMessage msgReceive();
    void stop();
    
    private:
    std::mutex                  mMutex;
    std::condition_variable     mCond;
    std::queue<InternalMessage> mQueue;
    std::atomic_bool            mTerminate;
    //following forbidden
    public:
    InternalMsgQueue* operator&();//get address operator
    const InternalMsgQueue* operator&()const;//get const address operator
    InternalMsgQueue(const InternalMsgQueue&) = delete;//copy construct
    InternalMsgQueue& operator=(const InternalMsgQueue&) = delete;//assign operator
    InternalMsgQueue(InternalMsgQueue&&) = delete;//move construct
    InternalMsgQueue& operator=(InternalMsgQueue&&) = delete;//move asssign construct
};

#endif
