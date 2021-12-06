#ifndef _IRAIN_HPP_
#define _IRAIN_HPP_

#include <vector>
#include <iostream>

class IRain{
    public:
        virtual ~IRain() = default;
        virtual int collectedRain(std::vector<int>& p_wall) = 0;
};

#endif
