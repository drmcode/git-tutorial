#ifndef _SP_DEBUG_H_
#define _SP_DEBUG_H_

#include <iostream>
#include <sstream>
//#include <fstream>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define SP_TRACE(level, text) { \
    std::ostringstream oss; \
    oss.setf(std::ios_base::boolalpha, std::ios_base::showbase); \
    oss<<__FILENAME__<<":"<<__LINE__<<":"<<__FUNCTION__<<"()"; \
    oss<<text<<std::endl; \
    std::cout<<oss.str().c_str(); \
}

#endif
