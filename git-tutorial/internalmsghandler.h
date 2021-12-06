#ifndef _INTERNALMSGHANDLER_H_
#define _INTERNALMSGHANDLER_H_
#include "imsghandler.h"
#include "iservice.h"

class InternalMsgHandler:public IMsgHandler
{
    public:
        InternalMsgHandler(std::shared_ptr<IService> baseFactory);
        ~InternalMsgHandler();
        void msgLoop();
    private:
        std::shared_ptr<InternalMsgQueue> m_internalMsgQueue;
};

#endif
