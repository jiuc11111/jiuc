#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include "common.h"


typedef struct FontBitmap_t
{
    Region tRegion;  //单个字符坐标信息
    int iCurOriginX;  //当前字符原点
    int iCurOriginY;
    int iNextOriginX; //下一个字符原点
    int iNextOriginY;
    unsigned char *bitmapBuff;  //渲染出的位图信息（即像素点0或1）
}FontBitmap,*pFontBitmap;

typedef struct FontOpr_t
{
    char *FontLibName;  //标准字库还是freetype
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