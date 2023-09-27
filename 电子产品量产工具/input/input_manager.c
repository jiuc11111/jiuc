#include "input_manager.h"
#include <pthread.h>
#include <stdbool.h>
 #include <stdio.h>

static pdeviceInput g_ptdeviceInput = NULL; //接收底层传输信息结构体
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER; //互斥量初始化
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER; //互斥量条件变量初始化

/*环形buff start*/
#define BUFF_SIZE 20
static InputEvent buff_data[BUFF_SIZE] = {0}; //接收底层输入信息缓存
static int g_iWrite = 0;
static int g_iRead = 0;

/*缓存是否满*/
static bool IsBuffDataFull(void)
{
    return (g_iRead == (g_iWrite + 1)%BUFF_SIZE);
}

/*缓存是否空*/
static bool IsBuffDataEmpty(void)
{
    return (g_iRead == g_iWrite);
}

/*将底层输入写入缓存*/
static int WriteBuffData(InputEvent tInputEvent)
{
    if(!IsBuffDataFull())
    {
        buff_data[g_iWrite] = tInputEvent;
        g_iWrite = (g_iWrite + 1)%BUFF_SIZE;
        return 0;
    }
    return -1;
}

/*读取缓存数据*/
static int ReadBuffData(pInputEvent ptInputEvent)
{
    if(!IsBuffDataEmpty())
    {
        *ptInputEvent =  buff_data[g_iRead];
        g_iRead = (g_iRead + 1)%BUFF_SIZE;
        return 0;
    }
    return -1;
}
/*环形buff end*/

/*底层输入接口，通过该函数将底层信息传输到中间层（此层）*/
void InputDeviceRegister(pdeviceInput ptdeviceInput)
{
    ptdeviceInput->ptNext = g_ptdeviceInput;
    g_ptdeviceInput = ptdeviceInput;
}

/*应用层接口，获取底层信息到中间层*/
void InputSystemRegister(void)
{
    extern void netInputRegister(void);
    netInputRegister();

    extern void touchscreenInputRegister(void);
    touchscreenInputRegister();   
}

/*线程处理函数*/
void *InputDevice_thread_func(void* data)
{
    InputEvent  event;
    pdeviceInput tmp = (pdeviceInput)data;
    int ret = 0;

    while(1)
    {
       ret = tmp->deviceGetInfo(&event);
        if(!ret)
        {
            /*保存数据*/
            pthread_mutex_lock(&g_tMutex);
            WriteBuffData(event); 

            pthread_cond_signal(&g_tConVar); //底层有输入，唤醒等待的进程
            pthread_mutex_unlock(&g_tMutex);          
        }
    }
    return NULL;
}

/*应用层接口，初始化为每个输入设备开辟一个线程*/
int InputDeviceInit(void)
{
	pthread_t tid;
	int ret;
    pdeviceInput ptemp = g_ptdeviceInput;        
    while(ptemp )
    {
        ret = ptemp->deviceInit();
        if (!ret)
        {
            ret = pthread_create(&tid, NULL, InputDevice_thread_func, (void *)ptemp);   //
        }
        ptemp = ptemp->ptNext;
    }
    return 0;
}

/*应用层接口，获取每个设备信息*/
int GetInfoInputDevice(pInputEvent ptInputDevice)
{
    int ret = 0;
    InputEvent tEvent;
    pthread_mutex_lock(&g_tMutex);
    if(!IsBuffDataEmpty())
    {
        ReadBuffData(&tEvent);
        *ptInputDevice = tEvent;
        pthread_mutex_unlock(&g_tMutex);
        ret = 0;
    }
    else
    {
        pthread_cond_wait(&g_tConVar, &g_tMutex);//等待条件变量
        if(!IsBuffDataEmpty())
        {
            ReadBuffData(&tEvent);
            *ptInputDevice = tEvent;
            pthread_mutex_unlock(&g_tMutex);
            ret = 0;           
        }
        else
        {
            pthread_mutex_unlock(&g_tMutex);
            ret = -1;
        }
    }

    return ret;
}

