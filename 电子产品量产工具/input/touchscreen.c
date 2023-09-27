#include "input_manager.h"
#include <tslib.h>
#include <stdio.h>

static struct tsdev *g_ts;

static int touchscreenDeviceInit(void)
{
    g_ts = ts_setup(NULL, 0); //初始化，扫描系统打开设备结点
    if(g_ts == 0)
    {
        printf("ts_setup err\n");
        return -1;
    }
    return 0;
}


static int touchscreenDeviceExit(void)
{
    ts_close(g_ts);
    return 0;
}

static int touchscreenDeviceGetInfo(pInputEvent ptInputEvent)
{
    int ret;
    struct ts_sample samp;
    ret = ts_read(g_ts,&samp,1); //读取触摸屏数据
    if(ret != 1)
    {
        printf("ts_read err\n");
        return -1;
    }
    ptInputEvent->type = INPUT_TYPE_TOUCH;
    ptInputEvent->x = samp.x;
    ptInputEvent->y = samp.y;
    ptInputEvent->Pressure = samp.pressure;
    ptInputEvent->time = samp.tv;
    return 0;
}

deviceInput g_touchInputEvent = 
{
    .name = "touchscreen",
    .deviceInit = touchscreenDeviceInit,
    .deviceExit = touchscreenDeviceExit,
    .deviceGetInfo = touchscreenDeviceGetInfo,
};

void touchscreenInputRegister(void)
{
    InputDeviceRegister(&g_touchInputEvent);
}

#if 0

int main(int argc, char **argv)
{
    InputEvent event;
    int ret = 0;

    g_InputEvent.deviceInit();



    while(1)
    {
       ret =  g_InputEvent.deviceGetInfo(&event);
        if(ret)
        {
            printf("touchscreenDeviceGetInfo err\n");
            return -1;
        }
        else
        {
            printf("type        = %d\n",event.type);
            printf("x           = %d\n",event.x);
            printf("y           = %d\n",event.y);
            printf("Pressure    = %d\n",event.Pressure);
            printf("time        = %d\n",event.time);
        }
    }
    g_InputEvent.deviceExit();
    return 0;
}

#endif