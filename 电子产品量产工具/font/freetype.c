#include "font_manager.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

FT_Face       g_FontFace;  //�õ������ļ�
unsigned int g_font_size = 12; //Ĭ�������С

/*��������������ռ����С������ÿ���ַ������ϸ�����������Сֵ*/
int compute_string_bbox(char* str, pDescartesRegion ptDescartesRegion)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_FontFace->glyph;

    /* ��ʼ�� */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* ָ��ԭ��Ϊ(0, 0) �����ַ������ԭ����Ϊ�˵õ�bbox.xMin�����ԭ���ֵ����LCD����ϵ�͵ѿ�������ϵ�޹�*/
    pen.x = 0;
    pen.y = 0;

    /* ����ÿ���ַ���bounding box */
    /* ��translate, ��load char, �Ϳ��Եõ���������� */
    for (i = 0; i < strlen(str); i++)
    {
        /* ת����transformation */
        FT_Set_Transform(g_FontFace, 0, &pen);

        /* ����λͼ: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_FontFace, str[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* ȡ��glyph */
        error = FT_Get_Glyph(g_FontFace->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* ��glyph�õ����: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* ������� */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* ������һ���ַ���ԭ��: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    ptDescartesRegion->iLeftUpX = bbox.xMin;
    ptDescartesRegion->iLeftUpY = bbox.yMax;
    ptDescartesRegion->iWidth   = bbox.xMax - bbox.xMin + 1;
    ptDescartesRegion->iHeigh   = bbox.yMax - bbox.yMin + 1;

    return 0;
}


/*��ʼ��freetype*/
static int freetypeFontInit(char* FontName)
{
    FT_Library    library;   
    int error;

    /*�̶�����*/
    error = FT_Init_FreeType( &library );              /* ��ʼ������� */
    if (error)
	{
		printf("FT_Init_FreeType error\n");
		return -1;
	}
    
    error = FT_New_Face( library, FontName, 0, &g_FontFace ); /* �õ������ļ�face */ 
    if (error)
	{
		printf("FT_New_Face error\n");
		return -1;
	} 

    FT_Set_Pixel_Sizes(g_FontFace, g_font_size, 0); //����Ĭ�������С
    return 0;
}

/*���������С*/
static int freetypeFontSize(unsigned int size)
{
    return FT_Set_Pixel_Sizes(g_FontFace, size, 0); //ָ�������С
}

/*����ַ�λͼ���ݣ�ԭ�㣬���Ͻ����꣬λͼ�ȵȣ����Ҹ�����һ���ַ�λͼ��Ϣ*/
static int freetypeGetFontBitmap(pFontBitmap ptFontBitmap,unsigned int dcode)
{
    int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_FontFace->glyph;

    pen.x = ptFontBitmap->iCurOriginX * 64; /* ��λ: 1/64���� */
    pen.y = ptFontBitmap->iCurOriginY * 64;/* ��λ: 1/64���� */

    /* ת����transformation */
    FT_Set_Transform(g_FontFace, 0, &pen);

    /* ����λͼ: load glyph image into the slot (erase previous one) */
    error = FT_Load_Char(g_FontFace, dcode, FT_LOAD_RENDER);
    if (error)
    {
        printf("FT_Load_Char error\n");
        return -1;
    }

    ptFontBitmap->bitmapBuff       =  slot->bitmap.buffer;
    ptFontBitmap->tRegion.iLeftUpX = slot->bitmap_left;
    ptFontBitmap->tRegion.iLeftUpY = 2*ptFontBitmap->iCurOriginY - slot->bitmap_top;
    ptFontBitmap->tRegion.iWidth   = slot->bitmap.width;
    ptFontBitmap->tRegion.iHeigh   = slot->bitmap.rows;

    ptFontBitmap->iNextOriginX    = ptFontBitmap->iCurOriginX+ slot->advance.x/64;
    ptFontBitmap->iNextOriginY    = ptFontBitmap->iCurOriginY ;

    return 0;
}

static FontOpr g_tFontOpr = 
{
    .FontLibName   = "freetype",
    .FontInit      = freetypeFontInit,
    .FontSize      = freetypeFontSize,
    .GetFontBitmap = freetypeGetFontBitmap,
    .pNext         = NULL,
};


void FontRegister(void)
{
     FontDeviceRegister(&g_tFontOpr);
}

