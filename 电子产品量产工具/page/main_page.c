#include  "page_manager.h"
#include <stdio.h>
#include "config.h"
#include "display_manager.h"
#include <math.h>
#include "input_manager.h"
#include <string.h>
#include "ui.h"
#include "font_manager.h"
#include <stdlib.h>


static button g_ptbutton[ITEM_BUTTON_MAX] = {0};  //按钮结构体数组
static int buttonCount = 0;  //按钮个数

/*有输入后按钮处理函数*/
int OnbuttonDrawHandler(pbutton ptbutton, InputEvent tInputEvent)
{
     unsigned int color = BUTTON_BASE_RCOLOR;  
    char state[20] ;     //按钮状态
    char* strButton;      
    strButton = ptbutton->TestName;
    int button_state_index = 0;  //按钮状态下标
    char* str_button_state[3] = {"err","ok","percentage"};
    char commom[100];
    pItemButtonInfo ptItemButtonInfo;

    if(tInputEvent.type == INPUT_TYPE_TOUCH)
    {
        		/* 1.1 分辨能否被点击 */
		if (GetItemCfgByName(ptbutton->TestName)->touchState == 0)
			return -1;
        
        ptbutton->status = !ptbutton->status;
        if(ptbutton->status)
        {
            color = BUTTON_BASE_GCOLOR;
            button_state_index = 1;
        } 
            
    }
    else if(tInputEvent.type == INPUT_TYPE_NET)
    {
        sscanf(tInputEvent.str ,"%*s %s",state);
        printf("--%s--\n",state);
        if(strcmp(state , "ok") == 0)
        {
            color = BUTTON_BASE_GCOLOR;
            button_state_index = 1;
        }           
        else if(strcmp(state , "err") == 0)
        {
            color = BUTTON_BASE_RCOLOR;
            button_state_index = 0;
        }        
        else if(state[0] >= '0' && state[0] <= '9')
        {
            color = BUTTON_BASE_BCOLOR;
            strButton = state;
            button_state_index = 2;
        }    
        else 
            return -1;
    }
    else
    {
        return -1;
    }
        /*绘制底色*/
    DrawBaseColor(ptbutton->tRegion,color);

    /*绘制文字*/
    DrawText(strButton, ptbutton->tRegion ,BUTTON_TEST_COLOR);

    /*执行按钮对应命令*/
    ptItemButtonInfo = GetItemCfgByName(ptbutton->TestName);
    if(ptItemButtonInfo->command[0] != '\0')   
    {
        sprintf(commom,"%s %s",ptItemButtonInfo->command,str_button_state[button_state_index]);
        system(commom);
    }
    return 0;      
}

/*根据最长字符计算出合适字体大小，让字体显示好看*/
int CalcFontSize(void)
{
    int i = 0;
    int Name_len = -1;
    int max_len_index = 0;
    DescartesRegion tDescartesRegion;
    float kw,kh;

    /*找出字符最长的按钮*/
    for(i = 0; i < buttonCount; i++)
    {
       if(Name_len < (int)strlen(g_ptbutton[i].TestName))
       {
            Name_len = strlen(g_ptbutton[i].TestName);
            max_len_index = i;
       }           
    }
    /*设置字符大小为100，计算最长字符外框*/
    AppSetFontSize(100);
    compute_string_bbox(g_ptbutton[max_len_index].TestName, &tDescartesRegion);

    /*缩放字符外框和区域边框相同，求出缩放系数kw,kh*/
    kw = (1.0f*g_ptbutton[max_len_index].tRegion.iWidth)/tDescartesRegion.iWidth;
    kh = (1.0f*g_ptbutton[max_len_index].tRegion.iHeigh)/tDescartesRegion.iHeigh;

    if(kw > kh)
        kw = kh;
    /*反算出实际字体大小,用kh,kw中较小系数*/
    return kw*100*0.8;
}

/*绘制界面，每个按钮*/
static void ItemCreatePageButton(void)
{

    PFrameBuffer_data ptFrameBuffer_data;
    int xres,yres;
    int wight,hight;
    int LineButtonCount;
    int start_x , start_y;
    int rows;
    int i = 0 ,row,col;
    int pre_x, per_y;
    int FontSize;

    ptFrameBuffer_data = GetDisplayBuffer();
    buttonCount = GetItemCfgCount();

    /*获得单个按钮长宽*/
    xres = ptFrameBuffer_data->ix;
    yres = ptFrameBuffer_data->iy;
    wight = sqrt(1.0/0.618*(xres*yres/buttonCount));
    LineButtonCount = xres/wight + 1;
    wight = xres/LineButtonCount;
    hight = 0.618*wight;

    /*让整个界面相对于显示屏居中显示*/
    start_x = (xres - wight*LineButtonCount)/2;
    rows    = buttonCount/LineButtonCount;
    if(rows*LineButtonCount < buttonCount)
        rows++;

    start_y = (yres - hight*rows)/2;

    /*循环获得每个按钮区域等信息*/
    for(row = 0 ;row < rows && (i < buttonCount); row++)
    {
        pre_x = start_x;
        per_y = start_y + hight*row;
        for(col = 0; col < LineButtonCount && (i < buttonCount); col++)
        {
            g_ptbutton[row*LineButtonCount + col].tRegion.iLeftUpX = pre_x;
            g_ptbutton[row*LineButtonCount + col].tRegion.iLeftUpY = per_y;
            g_ptbutton[row*LineButtonCount + col].tRegion.iWidth = wight - GYO_X;
            g_ptbutton[row*LineButtonCount + col].tRegion.iHeigh = hight - GYO_Y;
            g_ptbutton[row*LineButtonCount + col].TestName = GetItemCfgByIndex(row*LineButtonCount + col)->name;
            pre_x = g_ptbutton[row*LineButtonCount + col].tRegion.iLeftUpX + wight;
            buttonInit(&g_ptbutton[row*LineButtonCount + col], NULL,NULL,NULL ,OnbuttonDrawHandler);
            i++;
        }
    }

    FontSize = CalcFontSize();
    /*循环绘制每个按钮*/
    for(i = 0;i < buttonCount; i++)
    {
        g_ptbutton[i].FontSize = FontSize;
        g_ptbutton[i].button_Draw(& g_ptbutton[i]);
    }
}

/*判断点击区域，也就是点击哪个按钮*/
static int IsButtonInRegister(int x, int y, pbutton ptbutton)
{
    if(x < ptbutton->tRegion.iLeftUpX || x > (ptbutton->tRegion.iLeftUpX + ptbutton->tRegion.iWidth))
        return -1;
    if(y < ptbutton->tRegion.iLeftUpY || y > (ptbutton->tRegion.iLeftUpY + ptbutton->tRegion.iHeigh))
        return -1; 
    return 0;   
}

/*根据点击区域或网络输入找到对应按钮*/
static pbutton FindButton(pInputEvent ptInputEvent)
{
    int i;
    char name[100];
    if(ptInputEvent->type == INPUT_TYPE_TOUCH)
    {
        for(i = 0; i < buttonCount; i++)
        {
            if(!IsButtonInRegister(ptInputEvent->x, ptInputEvent->y , &g_ptbutton[i]))
                return &g_ptbutton[i];
        }
    }
    else if(ptInputEvent->type == INPUT_TYPE_NET)
    {
        sscanf(ptInputEvent->str, "%s", name);
        for(i = 0; i < buttonCount; i++)
        {
            if(strcmp(name, g_ptbutton[i].TestName) == 0)
                return &g_ptbutton[i];
        }

    }
    else
        return NULL;
    return NULL;
}

/*主界面运行*/
static void  MainRun(void* page)
{
    InputEvent tInputEvent;
    pbutton  ptbutton = NULL;
    int ret;
    
    /*读取配置文件*/
    ret = ItemParseConfigFile();
    if(ret)
        return;

    /*根据配置文件生成界面按钮*/
    ItemCreatePageButton();

    while(1)
    {
        /*读取输入事件*/
        ret = GetInfoInputDevice(&tInputEvent);
        if(ret)
        {
            perror("GetInfoInputDevice err\n");
            continue;
        }
        /*根据输入事件找到按钮*/
        ptbutton = FindButton(&tInputEvent);

        printf("--%p--\n",ptbutton);
        /*运行按钮对应函数*/
        if (!ptbutton)
			continue;
        ptbutton->button_Draw_handler(ptbutton, tInputEvent);
    }
}


static PageAction g_tPageAction = 
{
    .PageName = "main",
    .PageHandler = MainRun,
};

void MainRegiter(void)
{
    MainPageDeviceRegister(&g_tPageAction);
}