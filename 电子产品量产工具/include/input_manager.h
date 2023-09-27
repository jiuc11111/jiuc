#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 1 // ����
#define INPUT_TYPE_NET   2 //����

/*�������������ʹ�������ͨ�ýṹ��*/
typedef struct InputEvent_t
{
    int type; //�������뻹�Ǵ�����
    int x;    
    int y;
    int time;
    int Pressure;  //ѹ��ֵ�����»����ɿ���
    char str[1024]; //�������紫���������
}InputEvent,*pInputEvent;

/*�����ͨ�õײ��ʼ���Լ������ݴ��䵽�м�㺯��*/
typedef struct deviceInput_t
{
    char *name;
    int (*deviceInit)(void);
    int (*deviceExit)(void);
    int (*deviceGetInfo)(pInputEvent ptInputEvent);
    struct deviceInput_t *ptNext;
}deviceInput,*pdeviceInput;


void InputDeviceRegister(pdeviceInput ptdeviceInput);
void InputSystemRegister(void);
int InputDeviceInit(void);
int GetInfoInputDevice(pInputEvent ptInputDevice);
#endif



