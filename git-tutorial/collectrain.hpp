#ifndef _COLLECT_RAIN_HPP_
#define _COLLECT_RAIN_HPP_

#include <memory>
#include <vector>
#include "IRain.hpp"

class rainNormal : public IRain
{
    public:
        rainNormal();
        ~rainNormal();
        int collectedRain(std::vector<int>& p_wall);
    private:
        int max(int& x, int& y){ return (x > y) ? (x) : (y); }
        int min(int& x, int& y) { return (x < y) ? (x) : (y); }
};

class rainDp : public IRain
{
    public:
        rainDp();
        ~rainDp();
        int collectedRain(std::vector<int>& p_wall);
    private:
        int max(int& x, int& y){ return (x > y) ? (x) : (y); }
        int min(int& x, int& y) { return (x < y) ? (x) : (y); }
};

class rainDoublePoint : public IRain
{
    public:
        rainDoublePoint();
        ~rainDoublePoint();
        int collectedRain(std::vector<int>& p_wall);
    private:
        int max(int& x, int& y){ return (x > y) ? (x) : (y); }
};

/***************
*https://blog.csdn.net/qq_41822647/article/details/89364939?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522162691795616780357227257%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=162691795616780357227257&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-6-89364939.first_rank_v2_pc_rank_v29&utm_term=%E6%8E%A5%E9%9B%A8%E6%B0%B4+%E5%8D%95%E8%B0%83%E6%A0%88&spm=1018.2226.3001.4187
*维护一个单调递减栈，当当前元素小于栈顶元素时，进栈，当大于栈顶元素时，说明可以构成一个凹槽，栈顶元素出栈，
*并判断栈顶元素两边柱子的最小高度，取最小高度作为凹槽的高度。直到碰到栈顶元素大于当前元素时再次进栈。
*思路还是要找到栈顶柱子两边的最小柱子的高度，然后乘上柱子之间的宽度就是凹槽盛水的容积。
**************/
class rainStack : public IRain
{
    public:
        rainStack();
        ~rainStack();
        int collectedRain(std::vector<int>& p_wall);
    private:
        int min(int& x, int& y) { return (x < y) ? (x) : (y); }
};

#endif
