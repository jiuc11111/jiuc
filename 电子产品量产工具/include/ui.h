#ifndef _UI_H_
#define _UI_H_

#include "common.h"
#include "input_manager.h"

#define BUTTON_TEST_COLOR  0xffffff  //白色字体
#define BUTTON_BASE_RCOLOR 0xff0000  //红色底色
#define BUTTON_BASE_GCOLOR 0x00ff00  //绿色底色
#define BUTTON_BASE_BCOLOR 0x0000ff  //蓝色底色

struct button_t;  //声明结构体

/*函数指针别名*/
typedef void (*BUTTON_DRAW_FUNCTION)(struct button_t *pbutton);   
typedef int (*BUTTON_DRAW_HANDLER_FUNCTION)(struct button_t *pbutton, InputEvent pInputEvent);

typedef struct button_t
{
    int FontSize;     //按钮文字大小
    char* TestName;   //要显示的文字
    int status;       //按钮状态
    Region tRegion;   //指定区域
    BUTTON_DRAW_FUNCTION button_Draw;     //绘制文字和底色(初始化界面状态)
    BUTTON_DRAW_HANDLER_FUNCTION button_Draw_handler;   //有输入改变界面状态
}button,*pbutton;

void buttonInit(pbutton ptbutton, char* TextName,PRegion ptRegion,
                BUTTON_DRAW_FUNCTION buttonDraw ,BUTTON_DRAW_HANDLER_FUNCTION buttonDrawHandler);  //初始化


#endif