#ifndef _ISERVICE_H_
#define _ISERVICE_H_
#include <memory>
#include "imsghandler.h"
#include "internalmsgqu.h"

class IService
{
    public:
        virtual ~IService() = default;
        virtual std::shared_ptr<IMsgHandler> getInternalMsgHandler() const = 0;
        virtual std::shared_ptr<InternalMsgQueue> getInternalMsgQueue() const = 0;
};
#endif
