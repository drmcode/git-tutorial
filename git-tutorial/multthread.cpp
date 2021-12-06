/**
 *  * thread2.c implementation
 *   *
 *    */
 
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
 
 
/**
 *  * 有一int型全局变量g_Flag初始值为0；
 *   * 在主线程中启动线程1，打印“this is thread1”，并将g_Flag设置为1
 *   　* 在主线称中启动线程2，打印“this is thread2”，并将g_Flag设置为2
 *    * 线程1需要在线程2退出后才能退出
 *    　* 主线程在检测到g_Flag从1变为2，或者从2变为1的时候退出
 *     *
 *      * 思路: 要求线程2必需先退出线程1才退出，可以在线程1中设置pthread_join(pid2,NULL)
 *       * 要求主线程能够检测到条件的变化，应该使用条件变量
 *        */
int g_flag = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
void *thread1_func(void *arg)
{
    printf("this is thread1 id : %lu\n",pthread_self());
    pthread_mutex_lock(&mutex);
    if(g_flag == 2) 
    {
        pthread_cond_signal(&cond);
    }
    g_flag = 1;
    pthread_mutex_unlock(&mutex);
         
    pthread_join(*(pthread_t *)arg,NULL);
    printf("leave thread1\n");
    pthread_exit(0);
}

void *thread2_func(void *arg)
{
    printf("this is thread2 id : %lu\n",pthread_self());
    pthread_mutex_lock(&mutex);
    if(g_flag == 1)
    {
        pthread_cond_signal(&cond);
    }
    g_flag = 2;
    pthread_mutex_unlock(&mutex);
    printf("leave thread2\n");
    pthread_exit(0);
}
 
int main() 
{
    pthread_t p1,p2;
    /*创建线程1*/
    pthread_create(&p1,NULL,thread1_func,(void *)(&p2));
    /*如果加了sleep函数，线程1执行了一小部分，然后随着线程2进行触发主线程退出了*/
    /*创建线程2*/
    pthread_create(&p2,NULL,thread2_func,NULL);
    sleep(1);

    pthread_mutex_lock(&mutex);

    if(g_flag == 0) 
        pthread_cond_wait(&cond,&mutex);

    pthread_mutex_unlock(&mutex);
    printf("leave main thread\n");
    return 0;
}
