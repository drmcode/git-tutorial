#include "internalmsghandler.h"

InternalMsgHandler::InternalMsgHandler(std::shared_ptr<IService> baseFactory)
{
    m_internalMsgQueue = baseFactory->getInternalMsgQueue();
    std::cout<<"InternalMsgHandler::m_internalMsgQueue use_count "<<m_internalMsgQueue.use_count()<<std::endl;
}
InternalMsgHandler::~InternalMsgHandler(){}

void InternalMsgHandler::msgLoop()
{
    while(1)
    {
        InternalMessage l_msg = m_internalMsgQueue->msgReceive();
        std::cout<<"l_msg.type "<<l_msg.type<<", data "<<l_msg.data<<std::endl;
        return;
    }
}

