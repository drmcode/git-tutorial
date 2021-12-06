#ifndef COMMTYPES_HPP
#define COMMTYPES_HPP

struct InternalMessage
{
    int  type;
    char data[256];
};

#ifdef __cplusplus
extern "C" {
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifdef __cplusplus
}
#endif

#endif