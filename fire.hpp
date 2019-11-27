#ifndef FIRE_H
#define FIRE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
#endif


/******************************************/
/******************LIST start**************/
/*                                        */
/******************************************/
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

#endif
