#include "sp.hpp"

#include <iomanip>//control signal need add this head file.
#include "collectrain.hpp"
//static class data parameter must been initialization out of class(not in construct function)
//there is defautl init value if no distinct for static data parameter of class
int SP::s_count = 0;

SP::SP():m_count(0),
m_factory(std::make_shared<BaseFactory>()),
m_rain(std::make_shared<rainNormal>()),
m_rainDp(std::make_shared<rainDp>()),
m_rainDPoint(std::make_shared<rainDoublePoint>()),
m_rainSt(std::make_shared<rainStack>())
{
    m_factory->createService();
    m_factory->getInternalMsgHandler();
    m_Queue.push(12);
    m_Queue.push(34);
}
SP::~SP(){}
void SP::list()
{
    std::unique_lock<std::mutex> lLock(m_mutex);
    std::cout<<std::setw(20)<<std::setfill('*')<<"test_sp list, m_count "<<std::hex<<m_count<<std::endl;
    SP_TRACE(1, "test_sp list, m_count false");
    int lNum = m_Queue.front();
    m_Queue.pop();
    m_condi.notify_all();
    //m_condi.notify_one();
    std::cout<<"m_Queue.size() "<<m_Queue.size()<<", lNum "<<lNum<<", m_Queue.front() "<<m_Queue.front()<<std::endl;
}
std::shared_ptr<int> SP::getCount()
{
    std::unique_lock<std::mutex> lLock(m_mutex);    
    std::cout<<"enter getCount"<<std::endl;
    //m_condi.wait(lLock);
    std::cout<<"return getCount"<<std::endl;

    return m_count;
}

std::shared_ptr<BaseFactory> SP::getFactory() const
{
    return m_factory;
}

std::shared_ptr<IRain> SP::getRain() const
{
    return m_rain;
}

std::shared_ptr<IRain> SP::getDpRain() const
{
    return m_rainDp;
}

std::shared_ptr<IRain> SP::getDPointRain() const
{
    return m_rainDPoint;
}

std::shared_ptr<IRain> SP::getStRain() const
{
    return m_rainSt;
}

BaseFactory::BaseFactory()
    :m_internalMsgQueue(nullptr),
     m_internalMsgHandler(nullptr)
{}
BaseFactory::~BaseFactory(){}
void BaseFactory::createService()
{
    std::cout<<"createService"<<std::endl;
    createInternalMsgQueue();
    std::cout<<"1 m_internalMsgQueue.use_count "<<m_internalMsgQueue.use_count()<<std::endl;
    createInternalMsgHandler();
    std::cout<<"2 m_internalMsgQueue.use_count "<<m_internalMsgQueue.use_count()<<std::endl;
}

void BaseFactory::createInternalMsgQueue()
{
    if (!m_internalMsgQueue)
    {
        m_internalMsgQueue = std::make_shared<InternalMsgQueue>();
    }
}

void BaseFactory::createInternalMsgHandler()
{
    if(!m_internalMsgHandler)
    {
        m_internalMsgHandler = std::make_shared<InternalMsgHandler>(shared_from_this());
    }
}

std::shared_ptr<InternalMsgQueue> BaseFactory::getInternalMsgQueue() const
{
    if(!m_internalMsgQueue)// createInternalMsgQueue();
    {
        std::cout<<"m_internalMsgQueue not init!"<<std::endl;
    }

    return m_internalMsgQueue;
}

std::shared_ptr<IMsgHandler> BaseFactory::getInternalMsgHandler() const
{
    if(!m_internalMsgHandler)
    {
        std::cout<<"m_internalMsgHandler not init!"<<std::endl;
    }

    return m_internalMsgHandler;
}

