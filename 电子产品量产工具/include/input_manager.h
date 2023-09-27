#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 1 // 触摸
#define INPUT_TYPE_NET   2 //网络

/*抽象出网络输入和触摸输入通用结构体*/
typedef struct InputEvent_t
{
    int type; //网络输入还是触摸屏
    int x;    
    int y;
    int time;
    int Pressure;  //压力值（按下还是松开）
    char str[1024]; //保存网络传输过来数据
}InputEvent,*pInputEvent;

/*抽象出通用底层初始化以及将数据传输到中间层函数*/
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



