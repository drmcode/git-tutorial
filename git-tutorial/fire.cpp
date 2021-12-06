#include<iostream>  
#include<cstring>
#include<cstdlib>
#include<tuple>
#include<pthread.h>
////#include<thread> 
#include<unistd.h>
#include<sys/wait.h>


#include "fire.hpp"
#include <sys/resource.h>
#include "sp.hpp"
#include "collectrain.hpp"

#include "fire_mem.hpp"


using namespace std; 

#define STACK_CONF "/mnt/c/jiancanx/D/CR/git-tutorial/stack.conf"
 
/******************************************/
/******************LIST start**************/
/*                                        */
/******************************************/
void list_init(List *list, void(*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destory;
    list->head = NULL;
    list->tail = NULL;
    
    return;
}

void list_destroy(List *list)
{
    void *data;
    
    while(list_size(list)>0)
    {
        if(list_rem_next(list, NULL, (void **)&data) == 0 &&
        list->destroy != NULL)
        {
            list->destroy(data);
        }
    }
    
    memset(list, 0, sizeof(List));
    return;
}

int list_ins_next(List *list, ListElmt *element, const void *data)
{
    ListElmt *new_element;
    
    if((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
        return -1;
    
    new_element->data = (void *)data;
    if(element == NULL)
    {
        if(list_size(list) == 0)
            list->tail = new_element;
        
        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        if(element->next == NULL)
            list->tail = new_element;
        
        new_element->next = element->next;
        element->next = new_element;
    }
    
    list->size++;
    
    return 0;
}

int list_rem_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;
    
    if(list_size(list) == 0)
        return -1;
    
    if(element == NULL)
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;
        
        if(list_size(list) ==1)
            list->tail = NULL;

    }
    else
    {
        if(element->next == NULL)
            return -1;
        
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;
        
        if(element->next == NULL)
            list->tail = element;
    }
    
    free(old_element);
    list->size--;
    return 0;
}

void test_strtok()
{
     char str[] = "now # is the time for all # good men to come to the # aid of  their country";
     char delims[] = "#";
     char *result = NULL;
     result = strtok( str, delims );
     while( result != NULL ) {
         printf( "result is \"%s\"\n", result );
         result = strtok( NULL, delims );
     }
}

void serialNumberCaps(char* input, char* output)
{
    char *p = output;

    if((NULL == input) || (NULL == output))
        return;

    strncpy(output, input, strlen(input));

    while (*p != '\0')
    {
        if(*p >= 'a' && *p <= 'z')
        {
           *p -= ('a' - 'A');
        }
        p++;
    }
}


void *say_hello(void *)
{
//char str[128] = {0};
char str1[18];
char str2[18];
char input[] = "a234567890";
char output[32]= {0};

sprintf(str1, "%02X%02X%02X%02X%02X", input[0], input[1], input[2], input[3], input[4]);
snprintf(str2, 11, "%02x%02x%02x%02x%02x", input[0], input[1], input[2], input[3], input[4]);

serialNumberCaps(input, output);

cout<<__FILE__<<": str2:"<<str2<<endl;
cout<<__FILENAME__<<": Hello Thread!"<<endl;

//strncat(output, str1, strlen(str1));
cout<<"output: "<<output<<endl;
return 0;
}


int getparentUri(char* uri, char* parentUri)
{
    char tmp_uri[256] = {0};
    char *p = NULL;
   // char *pp = NULL;
    int len = 0;
    int count = 0;
    
    if((NULL == uri)||(NULL == parentUri))
        return 0;

    p = strrchr(uri, '.');
    cout<<"p: "<<p<<", uri "<<uri<<endl;

    while(p)
    {
        count++;
        if(*(p+1) == '\0')
        {
            memset(tmp_uri, 0, sizeof(tmp_uri));
            len = strlen(uri) - strlen(p);
            memcpy(tmp_uri, uri, len);
            p = strrchr(tmp_uri, '.');
        }
        else if(isalpha(*(p + 1)))
        {
            if(count < 3)
            {            
                strncpy(parentUri, uri, strlen(uri));
                return 1;
            }
            
            memset(tmp_uri, 0, sizeof(tmp_uri));
            len = len - strlen(p);
            memcpy(tmp_uri, uri, len - 1);
            cout<<"tmp_uri"<<tmp_uri<<endl;
            p = strrchr(tmp_uri, '.');
            
            if(isdigit(*(p + 1)))
            {
                continue;
            }
            len = len - strlen(p);
            memset(tmp_uri, 0, sizeof(tmp_uri));
            memcpy(tmp_uri, uri, len - 1);
            p = strrchr(tmp_uri, '.');
            strncpy(parentUri, tmp_uri, strlen(tmp_uri));

            cout<<"tmp_uri "<<tmp_uri<<endl;
            return 1;
        }
        else if(isdigit(*(p + 1)))
        {
         cout<<"uri "<<uri<<", uri len: "<<strlen(uri)<<endl;
          memset(tmp_uri, 0, sizeof(tmp_uri));
          len = len - strlen(p) - 1;
          memcpy(tmp_uri, uri, len - 1);
          p = strrchr(tmp_uri, '.');
        }
    }

    return 0;
}

long int cfgRpc_getStackSize()
{
//    int ret = 0;
    int index = 0;
    char * p = NULL;
    char * pp = NULL;
    char buf[512] = {0};
    char stackconf[3][8] = {0};
    long int stacksize = 1;

    if(access(STACK_CONF,F_OK) != 0)
    {
        //TRACE(TRACE_DEBUG, APP_NAME, "%s not exist\n", STACK_CONF);
        return 0;
    }

    FILE* fs = fopen(STACK_CONF, "r");
    if (fs != NULL)
    {
        if(fgets(buf, 512, fs) == NULL)
        {
         //TRACE(TRACE_DEBUG, APP_NAME, "stack.conf is %s\n", buf);
         cout<<"stack.conf is "<<buf<<endl;
         return 0;
        }
        cout<<"stack.conf is "<<buf<<endl;
        fclose(fs);;
        p = strchr(buf, '*');
        pp = buf;
        while((NULL != p)&&(index<3))
        {
            strncpy(stackconf[index], pp, (p-pp));
            pp = p+1;
            p = strchr(pp, '*');
            cout<<"stackconf["<<index<<"] "<<stackconf[index]<<endl;
            index++;
        }
        strncpy(stackconf[index], pp, strlen(pp));
        //TRACE(TRACE_DEBUG, APP_NAME, "stacksize is %s*%s*%s\n", stackconf[0], stackconf[1], stackconf[2]);
        cout<<"stacksize is "<<stackconf[0]<<"*"<<stackconf[1]<<"*"<<stackconf[2]<<endl;
        //return (atoi(stackconf[0])*atoi(stackconf[1])*atoi(stackconf[2]));
         for(int i = 0; i< 3; i++)
         {
             if (0 == strlen(stackconf[i]))
                 break;

             stacksize *= atoi(stackconf[i]);
         }
         
         cout<<"stacksize is "<<stacksize<<endl;

         return stacksize;
    }
    else
    {
        //TRACE(TRACE_DEBUG, APP_NAME, "file: %s open failure\n", STACK_CONF);
        cout<<"file "<<STACK_CONF<<" open failure"<<endl;
    }
    return 0;
}

void Date::set(int y, int m, int d)
{
    year = y;
    month = m;
    day = d;
}

bool Date::isLeapYear()
{
    return((year%4 == 0) && (year%100!= 0)) || (year%400==0);
}

void Date::print()
{
    if(isLeapYear())
    {
        cout<<"Leap Year: "<<year<<" "<<month<<" "<<day<<endl;
    }
    else
    {
        cout<<"Not Leap Year: "<<year<<" "<<month<<" "<<day<<endl;
    }
}

Date operator+(const Date&a, const Date&b)
{
    Date t;
    t.set(a.year+b.year, a.month+b.month, a.day+b.day);
    return t;
}

void thread_task()
{
    std::cout<< "C11 hello thread"<<std::endl;
}

double cfgdbm_getValue(char *release, double input)
{
    char *pEnd = NULL;
    char *pRelease = NULL;
    int len = 0;
    double temp = input;
    char temprelease[256] = {0};
    strcpy(temprelease, release);

    pEnd = rindex(temprelease, '.');
    if(pEnd)
    {
        pRelease = strdup(pEnd+1);
        *(pEnd+1) = '\0';
    }
    if(NULL != pRelease)
    {
        len = strlen(pRelease);
    }else
    {
        len = 1;
    }

    for(int i=0; i<len;i++)
    {
        temp = temp*10;
    }
    if(pRelease) free(pRelease);

    return temp;
}

void func_person(Person& p)
{
    p.show();
}

newThread::newThread():m_threadId(-1),m_running(false)
{
    std::cout<<"constructor"<<std::endl;
}

newThread::~newThread()
{
    std::unique_lock<std::mutex>  l_lock(m_mutex);
    if(m_running.load())
    {
         stopThread();   
    }

    std::cout<<"destructor"<<std::endl;
}

void newThread::startThread(){
        std::unique_lock<std::mutex> l_lock(m_mutex);
        pthread_create(&m_threadId, nullptr, threadFunc, this);
        m_running.store(true);
}
void newThread::stopThread(){
        std::unique_lock<std::mutex> l_lock(m_mutex);
        pthread_join(m_threadId, nullptr);
        m_running.store(false);   
}

void newThread::joinThread(){
    std::unique_lock<std::mutex> l_lock(m_mutex);
    pthread_join(m_threadId, nullptr);
}

void newThread::detachThread(){
    std::unique_lock<std::mutex> l_lock(m_mutex);
    pthread_detach(m_threadId);
}

void * newThread::threadFunc(void *args)
{
    char buff[]="1 2 3 4 5 6 7 8 9";
    std::cout<<"enter threadFunc: "<<args<<std::endl;

    char *p = strtok(buff, " ");
    while(p)
    {
        std::cout<<__FUNCTION__<<": pid: "<<pthread_self()<<", p->"<<p<<std::endl;
        p = strtok(NULL, " ");
        sleep(1);
    }
    return nullptr;
}

int cfgdbm_str_replace(char *p_result, char* p_source, char* p_seach, char *p_repstr)
{  
    int c = 0;
    int repstr_leng = 0;
    int searchstr_leng = 0;
    char *p1;
    char *presult = p_result;
    char *psource = p_source;
    char *prep = p_repstr;
    char *pseach = p_seach;
    int nLen = 0;
    repstr_leng = strlen(prep);
    searchstr_leng = strlen(pseach);
    
    do
    {
        p1 = strstr(psource, p_seach);
        if (p1 == 0)
        {
            strcpy(presult, psource);
            break;
        }
        c++;
    
        nLen = p1 - psource;
        memcpy(presult, psource, nLen);
        memcpy(presult + nLen, p_repstr, repstr_leng);
        psource = p1 + searchstr_leng;
        presult = presult + nLen + repstr_leng;
    } while (p1);
    
    return c;
}

void lambda_test()
{
    std::vector<int> v{1,2,3,4,5};
    std::for_each(v.begin(), v.end(), [](const int n){std::cout<<n<<' ';});
    std::cout<<endl;
}

string StringSolution::longestCommonPrefix(vector<string>& strs)
{
    std::string out;
    if(strs.empty()) return out;

    for(std::size_t j = 0; j< strs[0].size(); j++){
        for(unsigned int i = 1; i< strs.size(); i++){
            if(j > strs[i].length()-1 || strs[i][j] != strs[0][j]){
               return out;
            }
        }
        out.append(1, strs[0][j]);
    }

    return out;
}

void baseString()
{
    StringSolution strs;
    std::vector<std::string> ss = {"flower", "flow", "flignt"};
    cout<<"logestPreFix: "<<strs.longestCommonPrefix(ss)<<endl;

#if 0
//crash coredump caused using %s printf int parameter.
    char test1[] = {"testtesttest"};
    int test2 = 1234567;

    printf("test1 char to int, is %d\n", test1);
    
    printf("test2, int to char is %s\n", test2);
#endif

    return;
}

void test()
{
    char b64_key[256] = {"KGFubjogMTIzNDU2KSwgKGFubjogMTIzNDU2KQ=="};
    int b64_len = strlen(b64_key)-1;
    int count_pending = 0;

    char temp[256] = {0};
    char paraName[256] = "InternetGatewayDevice.WANDevice.{i}.X_ALU-COM_Cellular.AccessPoint.{i}.X_ALU-COM_WorkMode";
    //int ret = cfgdbm_str_replace(temp, paraName, (char *)("{i}"), (char *)"%d");
    int arr[10];
    int (*pArr)[10] = NULL;
    InternalMessage l_msg = {2, "Init"};
    pArr = &arr;
    std::cout<<"pArr: "<<pArr<<std::endl;
    std::cout<<"b64_len: "<<b64_len<<std::endl;
    while(b64_key[b64_len] == '=')
    {
        std::cout<<"b64_key["<<b64_len<<"]: "<<b64_key[b64_len]<<std::endl;
        b64_len--;
        count_pending++;
    }
    b64_key[b64_len+1] = '\0';
    std::cout<<count_pending<<"AA"<<b64_key<<std::endl;

    char srcTmp[] = {"192.168.1.1"};
    char delim[] = {"."};
    char *p = strtok(srcTmp, delim);
    int a = atoi(p);
    std::cout<<"atoi(NULL): "<<a<<", p: "<<p<<std::endl;
    

    strcpy(temp, paraName);
    strcat(temp, ",TR069");
    //std::cout<<"ret is "<<ret<<", temp: "<<temp<<std::endl;
    std::cout<<"temp: "<<temp<<std::endl;

    SP tp;
    //vector<int> wall = {0,1,0,2,1,0,1,3,2,1,2,1};
    vector<int> wall = {0,1,0,2,1,0,1,3,2,1,2,1};
    tp.helloWorld();
    tp.list();
    shared_ptr<int> ct=tp.getCount();

    shared_ptr<IRain> rain = tp.getRain();   
    shared_ptr<IRain> dpRain = tp.getDpRain();
    shared_ptr<IRain> dPointRain = tp.getDPointRain();
    shared_ptr<IRain> stRain = tp.getStRain();
    rain = tp.getDpRain();

    cout<<"normal Rain "<<rain->collectedRain(wall)<<endl
        <<"dp rain: "<<dpRain->collectedRain(wall)<<endl
        <<"doublePoint rain: "<<dPointRain->collectedRain(wall)<<endl
        <<"stack rain: "<<stRain->collectedRain(wall)<<endl;


    std::shared_ptr<BaseFactory> l_fac = tp.getFactory();

    std::shared_ptr<InternalMsgQueue> l_msgQueu= l_fac->getInternalMsgQueue();
    std::shared_ptr<IMsgHandler> l_msgHandler = l_fac->getInternalMsgHandler();

    cout<<"l_msgQueu user_count "<<l_msgQueu.use_count()<<endl;
    l_msgQueu->msgSend(l_msg);
    l_msgHandler->msgLoop();

    cout<<"ct value is "<<ct.get()<<", sizeof(tp.ch) "<<sizeof(tp.ch)<<endl;

    rainNormal ra;
    rainDp raDp;
    rainDoublePoint raPo;
    rainStack raSt;
    int tets = ra.collectedRain(wall);
    int tets1 = raDp.collectedRain(wall);
    int tets2 = raPo.collectedRain(wall);
    int tets3 = raSt.collectedRain(wall);
    
    cout<<"rain "<<tets<<", rainDp "<<tets1<<", rainDoublePoint "<<tets2<<", rainStack "<<tets3<<endl;

    MemMgr * myMem = MemMgr::getInstance();
    char *p1 = (char *)myMem->mem_alloc(20);
    if (p1){
        mem_monitor_t monitor;
        strcpy(p, "test for memMgr....");
        cout<<p<<endl;
        myMem->mem_monitor(&monitor);
        cout<<"total_size: "<<monitor.total_size<<endl;
        cout<<"free_cnt: "<<monitor.free_cnt<<endl;
        cout<<"free_size: "<<monitor.free_size<<endl;
        cout<<"free_biggest_size: "<<monitor.free_biggest_size<<endl;
        cout<<"used_cnt: "<<monitor.used_cnt<<endl;
        cout<<"used_pct: "<<monitor.used_pct<<endl;
        cout<<"frag_pct: "<<monitor.frag_pct<<endl;

        myMem->mem_free((void *)p1);
    }
    
}

#define ALIGN_TO_WORD(x) (((x)/sizeof(int) + ((x)%sizeof(int)==0?0:1))*sizeof(int))

int knapsack::date = -1;



int main(int argc, char **argv)
{
    bool iscircle = false;
    ListNode l1(1);
    ListNode l2(2, &l1);
    ListNode l3(3, &l2);  
    ListNode l4(4, &l3); 
    ListNode l5(5, &l4);
    ListNode l6(6, &l5);
    ListNode l7(7, &l6);
    ListNode l8(8, &l7);
    ListNode l9(9, &l8);

    iscircle = MergerList::circle(&l3);
    cout<<(iscircle?"have circle":"no circle")<<endl;

    lambda_test();

    //test();
    baseString();
    return 1;

    unsigned char aa[2] = {"1"};
    /*aa[0] = 1;
    aa[1] = 2;
    aa[2] = 3;
    aa[3] = 4;
*/

    std::cout<<"sizeof(aa) "<<sizeof(aa)<<", aa[0] "<<(int)aa[0]<<", aa[1] "<<(int)aa[1]<<", aa[2] "<<(int)aa[2]<<", aa[3] "<<(int)aa[3]<<", aa[4] "<<(int)aa[4]<<std::endl;

     shared_ptr<string> str111=make_shared<string>("test");
    std::cout<<str111.get()->c_str()<<std::endl;

    for(int i = 0; i< argc; i++)
    {
         std::cout<<"argc is "<<argc<<", argv["<<i<<"] "<<argv[i]<<std::endl;  
    }

#if 0
    char mac[] = "EA:8A:63:52:97:52";
    char tmpMac[33] = {0};
    char finalMac[33] = {0};

    //sscanf(mac, "%2x:%2x:%2x:%2x:%2x:%2x", (unsigned int *)tmpMac, (unsigned int *)(tmpMac+1), (unsigned int *)(tmpMac+2), (unsigned int *)(tmpMac+3), (unsigned int *)(tmpMac+4), (unsigned int *)(tmpMac+5));
    //sscanf(mac, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", (unsigned int *)tmpMac, (unsigned int *)(tmpMac+1), (unsigned int *)(tmpMac+2), (unsigned int *)(tmpMac+3), (unsigned int *)(tmpMac+4), (unsigned int *)(tmpMac+5));
    //sscanf(mac, "%2x:%2x:%2x:%2x:%2x:%2x",tmpMac, (tmpMac+1), (tmpMac+2), (tmpMac+3), (tmpMac+4), (tmpMac+5));
    sscanf(mac, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",tmpMac, (tmpMac+1), (tmpMac+2), (tmpMac+3), (tmpMac+4), (tmpMac+5));

	 int j = 0;
        for(int i=0; i<(int )strlen(mac); i++)
	 {
	      if(mac[i] != ':')
	      {
	          tmpMac[j] = mac[i];
	          j = j+1;
	      }
        }

    sprintf(finalMac,"%02x:%02x:%02x:%02x:%02x:%02x",tmpMac[0],tmpMac[1],tmpMac[2],tmpMac[3],tmpMac[4],tmpMac[5]);
    std::cout<<finalMac<<std::endl;
    return 0;
#endif

    point po;
    po._tmain();
 
    std::cout<<"before reverse"<<std::endl;
    ListNode *p =  &l9;
    while(p)
    {
        std::cout<<p->val<<" ";
        p= p->next;
    }
    std::cout<<std::endl;

    MergerList::reverseByRange(&l9, 2, 6);

    std::cout<<"after reverse"<<std::endl;
    p =  &l9;
    while(p)
    {
        std::cout<<p->val<<" ";
        p= p->next;
    }
    std::cout<<std::endl;

    p = MergerList::loopMoveNNode(&l9, 33);
    std::cout<<"right move 3"<<std::endl;
    while(p)
    {
        std::cout<<p->val<<" ";
        p= p->next;
    }
    std::cout<<std::endl;

    vector<vector<int>> gifts={{1,3,1}, 
                               {1,5,1},
                               {4,2,1}};
    GiftMaxValue l_gfValue;
    std::cout<<l_gfValue.dp_maxGift(gifts)<<std::endl;
    std::cout<<l_gfValue.maxGift(gifts)<<std::endl;

    newThread thr;
    thr.startThread();

    char buff1[] = "a b c d e f g h i j k l";
    char*p_buff1 = strtok(buff1, " ");
    while (p_buff1)
    {
        cout<<pthread_self()<<", p_buff1->"<<p_buff1<<endl;
        p_buff1=strtok(NULL, " ");
        sleep(1);
    }


    thr.stopThread();

    char tmp[] = {"Hello"};
    Test1 t1(tmp);
    Test1 t2 = t1;
    t2.show();

    std::vector<int> prices{7,1,5,3,6,4};
    std::vector<int> rains{4,2,0,3,2,5};
    SStack sst;

    int ans = 0;//sst.maxProfit(prices);
    int dp = 0;//sst.maxProfitDp(prices);
    int r = sst.rain(rains);
    std::cout<<ans<<", dp "<<dp<<", rain "<<r<<std::endl;

    BasicCString bs;

    char *bsp = new char[10];
    char src[] = {"Helloo0o!"};
    //bs.myMemcpy(static_cast<void *>(bsp), static_cast<void *>(src), 9);
    bs.myStrcpy(bsp, static_cast<const char*>(src));
    std::cout<<bsp<<std::endl;

    std::vector<std::string> str = {"10", "0001", "111001", "1", "0"};
    knapsack mybage;
    int result = -1;
    result = mybage.findMaxForm(str, 5, 3);

    std::cout<<"result is "<<result<<std::endl;

    std::map<int, int> st = {{2, 6}, {1, 3}, {4, 6}, {3, 3}, {5, 7}};
    result = mybage.findMaxValue(st, 10);
    std::cout<<"knapsack result is "<<result<<std::endl;

    std::vector<std::pair<int, int>> st1 = {{2, 6}, {1, 3}, {4, 6}, {3, 3}, {5, 7}};
    result = mybage.findMaxValue(st1, 10);
    std::cout<<"knapsack vector result is "<<result<<std::endl;

    std::string str1{10};
    string str2;

    std::cout<<"str size is "<<str1.size()<<", str1 "<<str1<<std::endl;

    std::vector<std::map<int, int> > stuffss = {{{2, 6}, {1, 3}, {4, 6}},
                                                 {{1,5},{3,8},{2, 9}}};
    //for(int i = 0; i< static_cast<int>(stuffss.size()); i++)
    /*
    for(auto&a: stuffss)
    {
        for(auto&b: a)
        std::cout<<"first "<<b.first<<", sencod "<<b.second<<std::endl;
    }

    std::map<int, int> stuffs = {{2, 6}, {1, 3}, {4, 6}};
    for(auto &a: stuffs)
    {
        std::cout<<" first "<<a.first<<", second "<<a.second<<std::endl;
        //system("./while.sh");
    }

    std::vector<int> vec = {1,2,3,4};

    for(auto&a: vec)
    {
        std::cout<<"a "<<a<<std::endl;
    }
*/

    #if 0
    pthread_t ppid[5];

    std::thread t(thread_task);
    t.detach();

    for(int i=0; i<5;i++)
    {
        int ret = pthread_create(&ppid[i], NULL, say_hello, NULL);
        if(ret)
        {
            cout<<"Error: can't create thread! "<<endl;
        }
        cout<<"index "<<i<<", pid is "<<ppid[i]<<endl;
    }
    
    List *p = (List *)malloc(sizeof(List));
    char * data1 = (char *)malloc(sizeof("test1"));
    strncpy(data1, "test1", sizeof("test1"));
  
    list_init(p, destory);
    list_ins_next(p, NULL, data1);
    
    cout<<(char *)(list_data(p->head))<<endl;
    cout<<sizeof(timeZoneData_CAUO)/sizeof(timeZoneData_CAUO[0])<<endl;
    cout<<reverse(214748)<<endl;
    //cout<<sizeof(array)<<endl;
    //cout<<strlen(array)<<endl;
    cout<<sizeof("12345")<<endl;
#endif

#if 0
//32092598212044628
    tuple<int, bool, string> t1 = tuple<int, bool, string>(11, true, "OK");
    tuple<int, bool, string> t2(t1);
    char array[10]="12345";
    char url[256] = {0};
    sprintf(url, "%s", "https://192.168.1.254:501005");
    int fd;
    char uri[256] = {0};
    char tmp[] ="InternetGateWayDevice.Firewall.X_Asb_chain.0.test.0.";
    char buf[512] = {0};
    char value[3][8] = {0};
    int index = 0;
    char * p = NULL;
    char *pp = NULL;
    long int stacksize = 1;
    struct rlimit rl;
    int result;
    typedef struct 
{
    char timeZone[8];
    char timeZoneName[64];
    char timeZonePosix[256];/* for POSIX LocalTimeZone, This field used for notify tr181 when update timezone from web */
} TimeZoneItem;
typedef struct
{
    char OPID[32];
    TimeZoneItem TZ[32];
}TimeZoneList;

TimeZoneList timezonelist[] =
{
    {
     "CAUO",
     {
          //{"+08:00", "Perth", "AWST-8"},
          {"+08:45", "Eucla", "CWST-845"},
          {"+09:30", "Darwin", "ACST-9:30"},
          {"+10:00", "Canberra, Melbourne, Sydney", "AEST-10AEDT,M10.1.0/2,M4.1.0/3"},
          {"+10:30", "Adelaide", "ACDT-10:30"},
          {"+11:00", "Canberra,Melbourne,Sydney", "AEDT-11"}
     }
    },
    {
     "SALT",
     {
          {"+01:00", "Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna", "CET-1CEST,M3.5.0/2,M10.5.0/3"}
     }
    }
};

int timeZoneListNum = sizeof(timezonelist)/sizeof(timezonelist[0]);
int tzItermNum = sizeof(timezonelist[0].TZ)/sizeof(timezonelist[0].TZ[0]);

cout <<"timeZoneListNum "<<timeZoneListNum<<", tzItermNum "<<tzItermNum<<endl;

    Date d;
    d.set(2020, 8, 10);
    Date d1;
    d1.set(1001, 2, 1);
    operator+(d,d1).print();
/*
    char delims[] = "#";
    char *result1 = NULL;

     char str[] = "now # is the time for all # good men to come to the # aid of their country";
     result1 = strtok( str, delims );
     while( result1 != NULL ) {
         printf( "result is \"%s\"\n", result1 );
         result1 = strtok( NULL, delims );
     }
     */
        rlim_t kStackSize = 64 * 1024 * 1024;
    result = getrlimit(RLIMIT_STACK, &rl);
    
    cout<<"result is "<<result<<" stack is "<<rl.rlim_cur<<" kStacksize "<<kStackSize<<endl;
    
    rl.rlim_cur = kStackSize;
    result = setrlimit(RLIMIT_STACK, &rl);
    
    result = getrlimit(RLIMIT_STACK, &rl);
    
    cout<<"result is "<<result<<" 2stack is "<<rl.rlim_cur<<" kStacksize "<<kStackSize<<endl;


    //cfgRpc_getStackSize();
    //fd = getparentUri(tmp, uri);
    //cout<<"uri: "<<uri<<endl;
#endif
#if 0
char value[256]={0};
    if(access("/mnt/c/jiancanx/D/CR/git-tutorial/stack.conf", F_OK) == 0)
    {
        FILE* fs = fopen("/mnt/c/jiancanx/D/CR/git-tutorial/stack.conf", "r");
        if(NULL != fs)
        {
            if(fgets(buf, 512, fs) != NULL)
            {
                cout<<"buf is "<<buf<<endl;

                p = strchr(buf, '*');
                pp = buf;
                while((NULL != p)&&(index<3))
                {
                    strncpy(value[index], pp, (p-pp));
                    pp = p+1;
                    p = strchr(pp, '*');
                    cout<<"value["<<index<<"] "<<value[index]<<endl;
                    index++;
                }
                strncpy(value[index], pp, strlen(pp));
                cout<<"value["<<index<<"] "<<value[index]<<endl;
            }
            
            
            for(int i = 0; i< 3; i++)
            {
                if (0 == strlen(value[i]))
                    break;

                stacksize *= atoi(value[i]);
            }
            //stacksize = atoi(value[0])*atoi(value[1])*atoi(value[2]);
            cout<<"buf after while is "<<buf<<", stacksize is "<<stacksize<<endl;
            fclose(fs);
            
            return stacksize;
        }
        else
        {
            cout<<"error file open failure."<<endl;
        }
    }
#endif
    
    //return 0;
    

#if 0
    int * p_test = (int *)malloc(sizeof(*p_test));
    
    char negativeNum[10][20] = {0};
    
    for (int i = 0; i<10; i++)
    {
       cout<<"negativeNum["<<i<<"]="<<negativeNum[i]<<" strlen="<<strlen(negativeNum[i])<<endl;
    }

    int port = atoi(strrchr("12:10", ':')+1);
    
    cout<<"atoi(blank) = "<<atoi("blank")<<endl;
    cout<<"port is "<<port<<endl;
    //cout<<"t1 contains:"<<std::get<0>(t1)<<", "<<std::get<2>(t1)<<endl;
#endif
#if 0
    pid_t pid;

    pid =fork();
    
    if((pid =fork()) == 0)
    {
        cout<<"child PID is "<<(int)getpid()<<endl;
        exit(0);
    }
    else if(pid > 0)
    {
        cout<<"parent PID is "<<(int)getpid()<<", child PID is "<<(int)pid<<endl;
    }
    else
    {
        cout<<"fork failure"<<(int)pid<<endl;
    }
    int status;
    waitpid(pid, &status, 0);
#endif


    return 0;
}  
/******************************************/
/******************LIST end**************/
/*                                        */
/******************************************/
