#include "ui.h"
#include "display_manager.h"
#include "font_manager.h"
#include "input_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc ,char** argv)
{
	int ret;
	//FontBitmap tFontBitmap;
    InputEvent tInputEvent;
    button tbutton;
    Region tRegion;

    if(argc != 2)
	{
		printf("Usage :%s <Font>\n",argv[0]);
		return -1;
	}
    Display_Init();
    select_DeviceName((unsigned char *)"fb");
    Get_Disp_Data();

	AppFontToManagerInit();
	ret = AppselectFontType("freetype",argv[1]);
	if(ret)
	{
		perror("AppselectFontType err\n");
		return -1;
	}
	
    tRegion.iLeftUpX = 100;
    tRegion.iLeftUpY = 100;
    tRegion.iWidth   = 300;
    tRegion.iHeigh  = 100;
    buttonInit(&tbutton, "Text",&tRegion,NULL ,NULL);
    tbutton.button_Draw(&tbutton);
	while(1)
	{
        tbutton.button_Draw_handler(&tbutton,tRegion, tInputEvent);
        sleep(2);
	}
  
    Display_Exit();
    return 0;
    
}
