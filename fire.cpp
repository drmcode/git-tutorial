#include<iostream>  
#include<cstring>
#include<cstdlib>
#include<tuple>
#include<pthread.h>
#include<thread> 


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

cout<<"Hellow Thread!"<<endl;
return 0;
}

int main()
{
//32092598212044628
    tuple<int, bool, string> t1 = tuple<int, bool, string>(11, true, "OK");
    tuple<int, bool, string> t2(t1);
    char array[10]="12345";
    test_strtok();
    char url[256] = {0};
    sprintf(url, "%s", "https://192.168.1.254:501005");

    int port = atoi(strrchr(url, ':')+1);
    
    cout<<"atoi(blank) = "<<atoi("blank")<<endl;
    cout<<"port is "<<port<<endl;
    cout<<"t1 contains:"<<std::get<0>(t1)<<", "<<std::get<2>(t1)<<endl;


    pthread_t pid[5];
    
    for(int i=0; i<5;i++)
    {
        int ret = pthread_create(&pid[i], NULL, say_hello, NULL);
        if(ret)
        {
            cout<<"Error: can't create thread! "<<endl;
        }
        cout<<"index "<<i<<", pid is "<<pid[i]<<endl;
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
    return 0;
}  
/******************************************/
/******************LIST end**************/
/*                                        */
/******************************************/
