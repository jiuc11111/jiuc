#include "ui.h"
#include "display_manager.h"


/*����Ĭ�ϳ�ʼ��ҳ��*/
void defaultbuttonDraw(pbutton ptbutton)  
{
    /*���Ƶ�ɫ*/
    DrawBaseColor(ptbutton->tRegion,BUTTON_BASE_RCOLOR);

    /*��������*/
   	AppSetFontSize(ptbutton->FontSize);
    DrawText(ptbutton->TestName, ptbutton->tRegion ,BUTTON_TEST_COLOR);
}

/*Ĭ�ϳ�ʼ��ҳ�水ť�仯����*/
int defaultbuttonDrawHandler(pbutton ptbutton, InputEvent pInputEvent)
{
    unsigned int color = BUTTON_BASE_RCOLOR;
    ptbutton->status = !ptbutton->status;
    if(ptbutton->status)
        color = BUTTON_BASE_GCOLOR;

        /*���Ƶ�ɫ*/
    DrawBaseColor(ptbutton->tRegion,color);

    /*��������*/
    DrawText(ptbutton->TestName, ptbutton->tRegion ,BUTTON_TEST_COLOR);    
    return 0;
}

/*ÿ����ť����ʼ��*/
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