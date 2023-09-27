#include "input_manager.h"
#include <pthread.h>
#include <stdbool.h>
 #include <stdio.h>

static pdeviceInput g_ptdeviceInput = NULL; //���յײ㴫����Ϣ�ṹ��
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER; //��������ʼ��
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER; //����������������ʼ��

/*����buff start*/
#define BUFF_SIZE 20
static InputEvent buff_data[BUFF_SIZE] = {0}; //���յײ�������Ϣ����
static int g_iWrite = 0;
static int g_iRead = 0;

/*�����Ƿ���*/
static bool IsBuffDataFull(void)
{
    return (g_iRead == (g_iWrite + 1)%BUFF_SIZE);
}

/*�����Ƿ��*/
static bool IsBuffDataEmpty(void)
{
    return (g_iRead == g_iWrite);
}

/*���ײ�����д�뻺��*/
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

/*��ȡ��������*/
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
/*����buff end*/

/*�ײ�����ӿڣ�ͨ���ú������ײ���Ϣ���䵽�м�㣨�˲㣩*/
void InputDeviceRegister(pdeviceInput ptdeviceInput)
{
    ptdeviceInput->ptNext = g_ptdeviceInput;
    g_ptdeviceInput = ptdeviceInput;
}

/*Ӧ�ò�ӿڣ���ȡ�ײ���Ϣ���м��*/
void InputSystemRegister(void)
{
    extern void netInputRegister(void);
    netInputRegister();

    extern void touchscreenInputRegister(void);
    touchscreenInputRegister();   
}

/*�̴߳�����*/
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
            /*��������*/
            pthread_mutex_lock(&g_tMutex);
            WriteBuffData(event); 

            pthread_cond_signal(&g_tConVar); //�ײ������룬���ѵȴ��Ľ���
            pthread_mutex_unlock(&g_tMutex);          
        }
    }
    return NULL;
}

/*Ӧ�ò�ӿڣ���ʼ��Ϊÿ�������豸����һ���߳�*/
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

/*Ӧ�ò�ӿڣ���ȡÿ���豸��Ϣ*/
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
        pthread_cond_wait(&g_tConVar, &g_tMutex);//�ȴ���������
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

