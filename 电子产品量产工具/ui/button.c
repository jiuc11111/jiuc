#include "ui.h"
#include "display_manager.h"


/*绘制默认初始化页面*/
void defaultbuttonDraw(pbutton ptbutton)  
{
    /*绘制底色*/
    DrawBaseColor(ptbutton->tRegion,BUTTON_BASE_RCOLOR);

    /*绘制文字*/
   	AppSetFontSize(ptbutton->FontSize);
    DrawText(ptbutton->TestName, ptbutton->tRegion ,BUTTON_TEST_COLOR);
}

/*默认初始化页面按钮变化后处理*/
int defaultbuttonDrawHandler(pbutton ptbutton, InputEvent pInputEvent)
{
    unsigned int color = BUTTON_BASE_RCOLOR;
    ptbutton->status = !ptbutton->status;
    if(ptbutton->status)
        color = BUTTON_BASE_GCOLOR;

        /*绘制底色*/
    DrawBaseColor(ptbutton->tRegion,color);

    /*绘制文字*/
    DrawText(ptbutton->TestName, ptbutton->tRegion ,BUTTON_TEST_COLOR);    
    return 0;
}

/*每个按钮出初始化*/
void buttonInit(pbutton ptbutton, char* TextName,PRegion ptRegion,BUTTON_DRAW_FUNCTION ibuttonDraw ,BUTTON_DRAW_HANDLER_FUNCTION ibuttonDrawHandler)
{
    if(TextName)
        ptbutton->TestName              = TextName;
    
    ptbutton->status                = 0;
    if(ptRegion)
        ptbutton->tRegion               = *ptRegion;
    
    ptbutton->button_Draw           = ibuttonDraw ? ibuttonDraw : defaultbuttonDraw;
    ptbutton->button_Draw_handler   = ibuttonDrawHandler ? ibuttonDrawHandler : defaultbuttonDrawHandler;

}