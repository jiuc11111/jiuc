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

FT_Face       g_FontFace;  //得到字体文件
unsigned int g_font_size = 12; //默认字体大小

/*计算整行文字所占外框大小，遍历每个字符，不断更新外框最大最小值*/
int compute_string_bbox(char* str, pDescartesRegion ptDescartesRegion)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_FontFace->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) ，和字符的相对原点是为了得到bbox.xMin相对于原点的值，与LCD坐标系和笛卡尔坐标系无关*/
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(str); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(g_FontFace, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_FontFace, str[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_FontFace->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* 计算下一个字符的原点: increment pen position */
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


/*初始化freetype*/
static int freetypeFontInit(char* FontName)
{
    FT_Library    library;   
    int error;

    /*固定步骤*/
    error = FT_Init_FreeType( &library );              /* 初始化字体库 */
    if (error)
	{
		printf("FT_Init_FreeType error\n");
		return -1;
	}
    
    error = FT_New_Face( library, FontName, 0, &g_FontFace ); /* 得到字体文件face */ 
    if (error)
	{
		printf("FT_New_Face error\n");
		return -1;
	} 

    FT_Set_Pixel_Sizes(g_FontFace, g_font_size, 0); //设置默认字体大小
    return 0;
}

/*设置字体大小*/
static int freetypeFontSize(unsigned int size)
{
    return FT_Set_Pixel_Sizes(g_FontFace, size, 0); //指定字体大小
}

/*获得字符位图数据（原点，左上角坐标，位图等等）并且更新下一个字符位图信息*/
static int freetypeGetFontBitmap(pFontBitmap ptFontBitmap,unsigned int dcode)
{
    int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_FontFace->glyph;

    pen.x = ptFontBitmap->iCurOriginX * 64; /* 单位: 1/64像素 */
    pen.y = ptFontBitmap->iCurOriginY * 64;/* 单位: 1/64像素 */

    /* 转换：transformation */
    FT_Set_Transform(g_FontFace, 0, &pen);

    /* 加载位图: load glyph image into the slot (erase previous one) */
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

