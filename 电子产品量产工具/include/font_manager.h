#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include "common.h"


typedef struct FontBitmap_t
{
    Region tRegion;  //�����ַ�������Ϣ
    int iCurOriginX;  //��ǰ�ַ�ԭ��
    int iCurOriginY;
    int iNextOriginX; //��һ���ַ�ԭ��
    int iNextOriginY;
    unsigned char *bitmapBuff;  //��Ⱦ����λͼ��Ϣ�������ص�0��1��
}FontBitmap,*pFontBitmap;

typedef struct FontOpr_t
{
    char *FontLibName;  //��׼�ֿ⻹��freetype
    int (*FontInit)(char* FontName);
    int (*FontSize)(unsigned int size);
    int (*GetFontBitmap)(pFontBitmap ptFontBitmap,unsigned int dcode);
    struct FontOpr_t* pNext;
}FontOpr,*pFontOpr;



void FontDeviceRegister(pFontOpr ptFontOpr);
int compute_string_bbox(char* str, pDescartesRegion ptDescartesRegion);
void FontSystemRegister(void);
int AppselectFontType(char* FontName,char* FontLibName);
int AppSetFontSize(unsigned int size);
int AppGetFontBitmap(pFontBitmap ptFontBitmap,unsigned int dcode);
#endif