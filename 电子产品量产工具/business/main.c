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
    /*�����ʼ��*/
    InputSystemRegister();  //ע������ϵͳ�������
    InputDeviceInit();      //��ʼ����Ӧ����

    /*��ʾ��ʼ��*/
    DisplaySystemRegister();   //ע����ʾϵͳ�������
    select_DeviceName((unsigned char *)"fb");   
    Get_Disp_Data();

    /*�����ʼ��*/
    FontSystemRegister();
	ret = AppselectFontType("freetype",argv[1]);
	if(ret)
	{
		perror("AppselectFontType err\n");
		return -1;
	}

    /*������*/
    MainPageSystemRegister();
    AppSelectPage("main")->PageHandler(NULL);
    
    return 0;
}
