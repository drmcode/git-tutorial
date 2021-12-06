#ifndef _IMSGHANDLE_H_
#define _IMSGHANDLE_H_
class IMsgHandler{
public:
    virtual ~IMsgHandler() = default;
    virtual void msgLoop() = 0;
};
#endif
