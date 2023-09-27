#include "font_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static pFontOpr g_pCurFontOpr = NULL; //当前选用字体库，是freetype还是标准库
static pFontOpr g_pFontOpr = NULL; //接收底层字体库信息


void FontDeviceRegister(pFontOpr ptFontOpr)
{
    ptFontOpr->pNext = g_pFontOpr;
    g_pFontOpr = ptFontOpr;
}

void FontSystemRegister(void)
{
   extern  void FontRegister(void);
    FontRegister();
}

/*选择标准字库还是freetype库,并且初始化字库*/
int AppselectFontType(char* FontName , char* FontLibName)
{
    pFontOpr temp = g_pFontOpr;

    while(temp)
    {
        if(!strcmp(temp->FontLibName,FontName))
            break;
        temp = temp->pNext;
    }
    if(!temp)
    {
        perror("selectFontType err");
        return -1;
    }

    g_pCurFontOpr = temp;  

    return g_pCurFontOpr->FontInit(FontLibName);
}

/*设置字体大小*/
int AppSetFontSize(unsigned int size)
{
    return g_pCurFontOpr->FontSize(size);
}

/*获得字体位图数据*/
int AppGetFontBitmap(pFontBitmap ptFontBitmap,unsigned int dcode)
{
    return g_pCurFontOpr->GetFontBitmap(ptFontBitmap,dcode);
}

