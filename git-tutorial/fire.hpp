#ifndef FIRE_H
#define FIRE_H

#include <stdlib.h>
#include <map>
#include <limits.h> //UINT_MAX
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <string>

//for socket
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <linux/netlink.h>

//for thread
#include <thread>
#include <atomic>
#include <mutex>

#ifdef __cplusplus
extern "C"
#endif

class point{
    public:
    int _tmain()
    {
        char *str = nullptr;

        getMemory(str, 5);
        
        strcpy(str, "hello");
        std::cout<<str<<std::endl;
        return 0;
    }
    void getMemory(char* &p, int num)
    {
        p = (char*)malloc(num*sizeof(char));
    }
};

class knapsack{
    static int date;
    public:

    int findMaxForm(std::vector<std::string>& strs, int m, int n)
    {
        int len = strs.size();

        int dp[100][100][100] = {0};
        
        for(int i = 1; i<= len; i++)
        {
            for(int j = 1; j<= m; j++)
            {
                for(int k = 1; k <= n; k++)
                {
                    dp[i][j][k] = std::max(dp[i][j][k], dp[i-1][j][k]);

                    int zero = 0;
                    int one = 0;

                    for(auto a: strs[i-1])
                    {
                        if(a == '0')
                            zero++;
                        else
                            one++;
                    }

                    if(((j - zero) >= 0) and ((k-one) >= 0))
                    {
                        dp[i][j][k] = std::max(dp[i-1][j][k], (dp[i-1][j-zero][k-one] + 1));
                    }
                }
            }
        }

        return dp[len][m][n];
    }

    int findMaxValue(std::map<int, int>& stuffs, int capacity){
        int len = stuffs.size();
        int dp[len+1][capacity+1];
        memset(dp, 0, (len+1+capacity+1)*sizeof(int));
        std::map<int,int>::iterator it = stuffs.begin();

        for(int i = 1; i<= len; i++)
        {
            int c = it->first;
            int v = it->second;

            for(int j=0; j<= capacity; j++)
            {
                if(c <= j)
                {
                    dp[i][j] = std::max(dp[i-1][j], (dp[i-1][j-c]+v));
                }
                else
                {
                    dp[i][j] = dp[i-1][j];
                }
            }
            it++;
        }
        return dp[len][capacity];
    }

    int findMaxValue(std::vector<std::pair<int, int>>& stuffs, int capacity){
        int len = stuffs.size();
        int dp[len+1][capacity+1];
        memset(dp, 0, (len+1+capacity+1)*sizeof(int));
        std::vector<std::pair<int,int>>::iterator it = stuffs.begin();

        for(int i = 1; i<= len; i++)
        {
            int c = it->first;
            int v = it->second;

            for(int j=0; j<= capacity; j++)
            {
                if(c <= j)
                {
                    dp[i][j] = std::max(dp[i-1][j], (dp[i-1][j-c]+v));
                }
                else
                {
                    dp[i][j] = dp[i-1][j];
                }
            }
            it++;
        }
        return dp[len][capacity];
    }
};

class BasicCString{
    public:
    void* myMemcpy(void* des, void* src, size_t len)
    {
        if((nullptr == des) ||(nullptr == src))
            return nullptr;
        
        const char* pSrc = static_cast<const char*>(src);
        char* pDes = static_cast<char*>(des);

        if((pDes > pSrc) and (pDes < (pSrc + len)))
        {
            unsigned int index = len -1;
            while(index >= 0)
            {
                pDes[index]=pSrc[index];
                index--;
            }
        }
        else
        {
            unsigned int index = 0;
            while(index<len)
            {
                pDes[index] = pSrc[index];
                index++;
            }
        }

        return des;
    }

    char* myStrcpy(char* des, const char* src)
    {
         if(!des || !src)
             return nullptr;
        
        char *d = des;
        const char *s = src;
         while(*s != '\0')
         {
             *d++ = *s++;
         }
         return des;
    } 
};

class StringSolution{
    public:
        std::string longestCommonPrefix(std::vector<std::string>& strs);
};

class SStack{
    public:
    int maxProfit(std::vector<int>& prices)
    {
        int ans = 0;
        std::vector<int> St;
        prices.emplace_back(-1);
        for(int i = 0; i< static_cast<int>(prices.size()); i++)
        {
            while(!St.empty() && St.back() > prices[i])
            {
               // std::cout<<"while: St.back() "<<St.back()<<", prices["<<i<<"]"<<prices[i]<<std::endl;
                ans = std::max(ans, St.back() - St.front());
                //std::cout<<"ans "<<ans<<std::endl;
                St.pop_back();
            }

            St.emplace_back(prices[i]);
            //std::cout<<"For: St.back() "<<St.back()<<", prices["<<i<<"]"<<prices[i]<<std::endl;

        }
        return ans;
    }

    int maxProfitDp(std::vector<int>& prices)
    {
        int index = 1;
        int ans = prices[0];
        std::vector<int> dp(prices.size(), 0);
        if(prices.size() == 0)
            return 0;

        for(auto& a: prices)
        {
           ans = std::min(ans, a);
           dp[index] = std::max(dp[index -1], (a - ans));
           index++;
        }
        return dp[index-1];
    }

    int rain(std::vector<int>& high)
    {
        std::vector<int> st;
        int current = 0;
        int ans = 0;

        while(current < static_cast<int>(high.size()))
        {
            while(!st.empty() && (high[st.back()] < high[current]))
            {
                int top = st.back();
                st.pop_back();
                if (st.empty())
                    break;
                int distance = current - st.back() - 1;
                int h = std::min(high[current], high[st.back()]) - high[top];
                ans += distance*h;
            }
            st.push_back(current++);
        }
        return ans;
    }
};

class Test1{
    public:
    Test1() = default;
    Test1(char * name)
    {
        m_name = new char[strlen(name)+1];
        strncpy(m_name, name, strlen(name));
    }
    virtual ~Test1()
    {
        if(m_name)
            delete[] m_name;
    }
    Test1(const Test1& t)
    {
        m_name = new char[strlen(t.m_name) + 1];
        memcpy(m_name, t.m_name, strlen(t.m_name));
    }
    Test1& operator=(const Test1& t)
    {
        if(this == &t)
            return *this;
        
        delete[] m_name;

        m_name = new char[strlen(t.m_name)];
        strcpy(m_name, t.m_name);
    }
    void show()
    {
        std::cout<<m_name<<std::endl;
    }
    private:
    char*m_name;
};

typedef void*(*pFuc)(void*);

class newThread{
    public:
    newThread();
    virtual ~newThread();
    void startThread();
    void stopThread();
    void joinThread();
    void detachThread();
    static void * threadFunc(void *args);

    private:
    pthread_t m_threadId;
    std::atomic_bool m_running;   
    std::mutex m_mutex;
    void * m_par;
};

class GiftMaxValue{
    public:
    int dp_maxGift(std::vector<std::vector<int>>& gifts)
    {
        int m = gifts.size();
        int n = gifts[0].size();

        std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
        for(int i = 1; i<= m; i++)
        {
            for(int j = 1; j<= n; j++)
            {
                dp[i][j] = std::max(dp[i-1][j], dp[i][j-1])+gifts[i-1][j-1];
                //std::cout<<"dp["<<i<<"]["<<j<<"]"<<dp[i][j]<<"  ";
            }
            //std::cout<<std::endl;
        }
        return dp[m][n]; 
    }

    /*dp without extend spece*/
    int maxGift(std::vector<std::vector<int>>& gifts)
    {
        int m = gifts.size();
        int n = gifts[0].size();
        //std::cout<<"m: "<<m<<", n:"<<n<<std::endl;
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

        for(int i = 0; i< m; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if((0 == i) and (0 == j)) 
                {
                    dp[0][0] = gifts[0][0];
                    continue;
                }
                if(0 == i)
                {
                    dp[0][j] = dp[0][j-1] + gifts[0][j];
                    continue;
                }
                if(0 == j)
                {
                    dp[i][0] = dp[i-1][0] + gifts[i][0];
                    continue;
                } 
                dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]) + gifts[i][j];
            }
        }
        //std::cout<<"m: "<<m<<", n:"<<n<<std::endl;

/*
        for(int i = 0; i< m; i++)
        {
            for(int j = 0; j< n; j++)
            {
                std::cout<<"dp["<<i<<"]["<<j<<"]"<<dp[i][j]<<"  ";
            }
            std::cout<<std::endl;
        }
*/
        return dp[m-1][n-1];
    }
};


/******************************************/
/******************LIST start**************/
/*                                        */
/******************************************/
/******************************************
*************micro definition**************
*/
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

/*Define a structure for linked list element*/
typedef struct ListElmt_
{
    void *data;
    struct ListElmt_ *next;
}ListElmt;

/*Define a structure for linked lists*/
typedef struct List_
{
    int size;
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    ListElmt *head;
    ListElmt *tail;
}List;

struct ListNode{
    int val;
    ListNode*next;
    ListNode(int val):val(val),next(nullptr){}
    ListNode(int val, ListNode*next):val(val),next(next){}
};

typedef struct 
{
    int  qctimezone;
    char timeZone[8];
    char timeZoneName[64];
    char timeZonePosix[256];/* for POSIX LocalTimeZone, This field used for notify tr181 when update timezone from web */
} TimeZoneItem;

  TimeZoneItem timeZoneData_CAUO[] = 
  {
      {32,"+08:00", "Perth", "AWST-8"},
      {40,"+10:00", "Canberra, Melbourne, Sydney", "AEST-10AEDT,M10.1.0/2,M4.1.0/3"},
      {44,"+11:00", "Solomon Is., New Caledonia", "SBT-11"}
  };
  
class Date{
int year, month, day;
public:
void set(int y, int m, int d);
bool isLeapYear();
void print();
friend Date operator+(const Date& a, const Date&b);
};

/*public interface*/
void list_init(List *list, void(*destroy)(void *data));
void list_destroy(List *list);
int list_ins_next(List *list, ListElmt *element, const void *data);
int list_rem_next(List *list, ListElmt *element, void **data);

#define list_size(list) ((list)->size)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head?1:0)
#define list_is_tail(element) ((element == NULL)?1:0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

/******************************************/
/******************LIST end**************/
/*                                        */
/******************************************/
int reverse(int x)
{
    int max = 0x7fffffff, min = 0x80000000;
    long rs = 0;
    for(;x;rs = rs*10+x%10,x/=10);
    return rs>max||rs<min?0:rs;
}

void destory(void *data)
{
    if(NULL != data)
    {
        free(data);
        data = NULL;
    }
}

class MergerList{
public:
    static bool circle(ListNode*head){
        if ((nullptr == head)||(nullptr == head->next)) return false;

        ListNode *slow = head;
        ListNode  *fast = head->next;

        while(fast)
        {
            if(nullptr == fast->next)
                return false;
            if(slow == fast)
                return true;
            
            slow = slow->next;
            fast = fast->next;
        }
        return false;
    }

    static ListNode * reverseByRange(ListNode* head, int start, int end)
    {
        if(!head|| !head->next || (end < start))
            return head;

        ListNode * dummy = new ListNode(-1);
        dummy->next = head;

        ListNode * pre = dummy;
        ListNode * cur = dummy->next;

        int interval = end - start;

        for (int i = 1; i < start; i++)
        {
            pre = pre->next;
            cur = cur->next;
        }

        while(interval--)
        {
            ListNode * tmp = cur->next;
            cur->next = cur->next->next;
            tmp->next = pre->next;
            pre->next = tmp;
        }

        head = dummy->next;
        free(dummy);

        return head;
    }

    static ListNode* loopMoveNNode(ListNode* head, int n)
    {
        if(!head)
            return head;

        int sum = 0;
        ListNode * p = head;
        ListNode * tail;

        while(p)
        {
            tail = p;
            p = p->next;
            sum++;
        }

        p = head;

        tail->next = p;

        for(int i = 0; i< (n%sum); i++)
        {
            tail = tail->next;
            p = p->next;
        }

        tail->next = nullptr;
        head = p;

        return head;
    }
};

class Person{
    public:
    virtual void show(){std::cout<<"Person"<<std::endl;}
};

class Childen: public Person
{
    public:
    void show(){
        std::cout<<"Childen: "<<std::endl;
    }
};

class Adult: public Person
{
public:
void show(){
    std::cout<<"Adult: "<<std::endl;
}
};

#endif
