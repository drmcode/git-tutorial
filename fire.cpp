#include<iostream>  
#include<cstring>
#include<cstdlib>
#include<tuple>

//thread related *.h
#include<pthread.h>
#include<thread> 

//socket related *.h
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include "fire.hpp"


using namespace std; 
 
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

    memset(list, 0, sizeof(list));
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

void *say_hello(void *)
{
    char test[64] ={0};
    int socket_fd;
    struct sockaddr_un name;
    memset(&name, 0, sizeof(name));
    
    socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(-1 == socket_fd)
    {
        cout<<"socket create failure"<<endl;
        return 0;
    }

    cout<<"socket create successful"<<endl;

    name.sun_family = AF_LOCAL;
    (void)connect(socket_fd, (struct sockaddr *)&name, sizeof(name));
    int index = 0;
    //while(1)
    {
        sprintf(test, "%d %s", index, "Hello Word! this is socket thread!");
        write(socket_fd, test, strlen(test));
        cout<<"send out "<<index<<endl;
        index++;
        sleep(2);
    }

    close(socket_fd);

    return 0;
}

void* handle_event(void*)
{
    int socket_fd;
    fd_set rset;
    struct timeval tv;
    int ready = 0;
    struct sockaddr_un name;
    struct sockaddr_un client_name;
    int client_fd;

    cout<<"enter hand_event thread"<<endl;

    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(socket_fd<0)
    {
        cout<<"socket create failure"<<endl;
    }

    socklen_t client_name_len = sizeof(struct sockaddr);
    memset(&name, 0, sizeof(name));

    name.sun_family = AF_LOCAL;
    if(bind(socket_fd, (struct sockaddr *)&name, sizeof(struct sockaddr_un)) != 0)
    {
        cout<<"failure to bind socket "<<socket_fd<<endl;
        close(socket_fd);
    }
    if(listen(socket_fd, 8))
    {
        cout<<"failure to listen socket "<<socket_fd<<endl;
    }

    FD_ZERO(&rset);
    FD_SET(socket_fd, &rset);

    while(1)
    {

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        char readbuf[256] = {0};
        int readlen = 0;

        cout<<"main loop"<<endl;

        ready = select(socket_fd+1, &rset, NULL, NULL, &tv);
        if(ready < 0)
        {
            cout<<"select not ready"<<endl;
            continue;
        }
        else
        {
            cout<<"ready is "<<ready<<endl;
            if(FD_ISSET(socket_fd, &rset))
            {
                client_fd = accept(socket_fd, (struct sockaddr*)&client_name, &client_name_len);
                if(client_fd < 0)
                {
                    cout<<"client_fd"<<endl;
                }
                readlen = read(client_fd, readbuf, sizeof(readbuf));
                if(readlen > 0)
                {
                    cout<<"socket "<<socket_fd<<" receive message is "<<readbuf<<endl;
                }
                else
                {
                    cout<<"read NULL "<<endl;
                }
            }
        }
        cout<<"continue..."<<endl;
    }

     cout<<"close socket and return"<<endl;
    close(socket_fd);
}

int main()
{
//32092598212044628
    tuple<int, bool, string> t1 = tuple<int, bool, string>(11, true, "OK");
    tuple<int, bool, string> t2(t1);
    char array[10]="12345";
    test_strtok();
    char url[256] = {0};
    int ret;
    sprintf(url, "%s", "https://192.168.1.254:501005");

    int port = atoi(strrchr(url, ':')+1);

    cout<<"atoi(blank) = "<<atoi("blank")<<endl;
    cout<<"port is "<<port<<endl;
    cout<<"t1 contains:"<<std::get<0>(t1)<<", "<<std::get<2>(t1)<<endl;


    pthread_t pid[5];

    for(int i=0; i<5;i++)
    {
        if (3 == i)
        {
            ret  = pthread_create(&pid[i], NULL, handle_event, NULL);
        }else
        {
            ret = pthread_create(&pid[i], NULL, say_hello, NULL);
        }

        if(ret)
        {
            cout<<"Error: can't create thread! "<<endl;
        }
        cout<<"index "<<i<<", pid is "<<pid[i]<<endl;
    }

    for(int i = 0; i<5; i++)
    {
        pthread_join(pid[i], NULL);
        cout<<"index "<<i<<" thread join"<<endl;
    }

    List *p = (List *)malloc(sizeof(List));
    char * data1 = (char *)malloc(sizeof("test1"));
    strncpy(data1, "test1", sizeof("test1"));

    list_init(p, destory);
    list_ins_next(p, NULL, data1);

    cout<<(char *)(list_data(p->head))<<endl;
    cout<<sizeof(timeZoneData_CAUO)/sizeof(timeZoneData_CAUO[0])<<endl;
    cout<<reverse(214748)<<endl;
    cout<<sizeof(array)<<endl;
    cout<<strlen(array)<<endl;
    cout<<sizeof("12345")<<endl;
    sleep(3);
    return 0;
}  
/******************************************/
/******************LIST end**************/
/*                                        */
/******************************************/
