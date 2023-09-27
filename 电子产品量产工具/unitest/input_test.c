
#include "input_manager.h"
#include <stdio.h>



int main(int argc ,char** argv)
{
    InputInit();
    InputDeviceInit();
    InputEvent event ;
    int ret = 0;

    while(1)
    {
       ret =  GetInfoInputDevice(&event);
        if(ret)
        {
            printf("GetInfoInputDevice err\n");
            return -1;
        }
        else
        {
            if(event.type == INPUT_TYPE_TOUCH)
            {
                printf("type        = %d\n",event.type);
                printf("x           = %d\n",event.x);
                printf("y           = %d\n",event.y);
                printf("Pressure    = %d\n",event.Pressure);
            }
            else if(event.type == INPUT_TYPE_NET)
            {
                printf("type        = %d\n",event.type);
                printf("str         =%s\n",event.str);
            }

        }
    }
    return 0;    
}