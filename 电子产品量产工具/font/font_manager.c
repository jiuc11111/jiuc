#include "font_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static pFontOpr g_pCurFontOpr = NULL; //��ǰѡ������⣬��freetype���Ǳ�׼��
static pFontOpr g_pFontOpr = NULL; //���յײ��������Ϣ


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

/*ѡ���׼�ֿ⻹��freetype��,���ҳ�ʼ���ֿ�*/
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

/*���������С*/
int AppSetFontSize(unsigned int size)
{
    return g_pCurFontOpr->FontSize(size);
}

/*�������λͼ����*/
int AppGetFontBitmap(pFontBitmap ptFontBitmap,unsigned int dcode)
{
    return g_pCurFontOpr->GetFontBitmap(ptFontBitmap,dcode);
}

