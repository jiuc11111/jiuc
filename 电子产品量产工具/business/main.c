#include "input_manager.h"
#include <stdio.h>
#include "display_manager.h"
#include "font_manager.h"
#include <stdlib.h>
#include "page_manager.h"


int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <font_file>\n",argv[0]);
        return -1;
    }

    int ret;
    /*输入初始化*/
    InputSystemRegister();  //注册输入系统到管理层
    InputDeviceInit();      //初始化对应输入

    /*显示初始化*/
    DisplaySystemRegister();   //注册显示系统到管理层
    select_DeviceName((unsigned char *)"fb");   
    Get_Disp_Data();

    /*字体初始化*/
    FontSystemRegister();
	ret = AppselectFontType("freetype",argv[1]);
	if(ret)
	{
		perror("AppselectFontType err\n");
		return -1;
	}

    /*主界面*/
    MainPageSystemRegister();
    AppSelectPage("main")->PageHandler(NULL);
    
    return 0;
}
