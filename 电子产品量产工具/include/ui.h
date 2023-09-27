#ifndef _UI_H_
#define _UI_H_

#include "common.h"
#include "input_manager.h"

#define BUTTON_TEST_COLOR  0xffffff  //��ɫ����
#define BUTTON_BASE_RCOLOR 0xff0000  //��ɫ��ɫ
#define BUTTON_BASE_GCOLOR 0x00ff00  //��ɫ��ɫ
#define BUTTON_BASE_BCOLOR 0x0000ff  //��ɫ��ɫ

struct button_t;  //�����ṹ��

/*����ָ�����*/
typedef void (*BUTTON_DRAW_FUNCTION)(struct button_t *pbutton);   
typedef int (*BUTTON_DRAW_HANDLER_FUNCTION)(struct button_t *pbutton, InputEvent pInputEvent);

typedef struct button_t
{
    int FontSize;     //��ť���ִ�С
    char* TestName;   //Ҫ��ʾ������
    int status;       //��ť״̬
    Region tRegion;   //ָ������
    BUTTON_DRAW_FUNCTION button_Draw;     //�������ֺ͵�ɫ(��ʼ������״̬)
    BUTTON_DRAW_HANDLER_FUNCTION button_Draw_handler;   //������ı����״̬
}button,*pbutton;

void buttonInit(pbutton ptbutton, char* TextName,PRegion ptRegion,
                BUTTON_DRAW_FUNCTION buttonDraw ,BUTTON_DRAW_HANDLER_FUNCTION buttonDrawHandler);  //��ʼ��


#endif