#ifndef SP_HPP
#define SP_HPP

#include <memory>
#include <iostream>
#include "internalmsghandler.h"
#include "sp_debug.hpp"
#include "collectrain.hpp"

class BaseFactory:public IService, public std::enable_shared_from_this<BaseFactory>
{
    public:
    BaseFactory();
    ~BaseFactory();
    void createService();
    std::shared_ptr<InternalMsgQueue> getInternalMsgQueue() const;
    std::shared_ptr<IMsgHandler> getInternalMsgHandler() const;
    private:
    void createInternalMsgQueue();
    void createInternalMsgHandler();
    private:
    std::shared_ptr<InternalMsgQueue> m_internalMsgQueue;
    std::shared_ptr<InternalMsgHandler> m_internalMsgHandler;
};


class SP{
    public:
    SP();
    ~SP();
    char ch[256];
    void helloWorld(){std::cout<<"sp_hello!"<<std::endl;}
    void list();
    std::shared_ptr<BaseFactory> getFactory() const;
    std::shared_ptr<int> getCount();
    std::shared_ptr<IRain> getRain() const;
    std::shared_ptr<IRain> getDpRain() const;
    std::shared_ptr<IRain> getDPointRain() const;
    std::shared_ptr<IRain> getStRain() const;

    private:
    std::shared_ptr<int> m_count;
    std::shared_ptr<BaseFactory> m_factory;
    std::queue<int>m_Queue;
    std::mutex m_mutex;
    std::condition_variable m_condi;
    static int s_count;//static data parameter
    std::shared_ptr<rainNormal> m_rain;
    std::shared_ptr<rainDp> m_rainDp;
    std::shared_ptr<rainDoublePoint> m_rainDPoint;
    std::shared_ptr<rainStack> m_rainSt;
};


#endif
