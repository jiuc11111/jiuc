#include <stdio.h>
#include <string.h>

#include "display_manager.h"

static PDispManager temp_PDispManager = NULL;  //存放底层传入的通用结构体信息
static PDispManager Now_PDispManager = NULL;  //当前设备信息
static FrameBuffer_data tFrameBuffer_data ;   //触摸屏信息
static int line_width;       //行字节数
static int pixel_width;      //一个像素多少个字节


/*在指定区域绘制指定底色*/
void DrawBaseColor(Region tRegion,unsigned int color)
{
    int  i, j, p, q;
	int x = tRegion.iLeftUpX;
	int y = tRegion.iLeftUpY;
    int  x_max = x + tRegion.iWidth; //LCD坐标系
    int  y_max = y + tRegion.iHeigh;//LCD坐标系
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
	            PutPixel(i, j, color);
        }
    }	
}


/*在指定区域居中显示指定颜色字体*/
void DrawText(char* textname, Region tRegion ,unsigned int color)
{
	int i = 0;
	int pen_x,pen_y;  //LCD坐标系原点
	int error;
	FontBitmap tFontBitmap;   
	DescartesRegion tDescartesRegion;  //保存文字外框边界信息

	/*计算出一行文字外框*/
	error = compute_string_bbox(textname, &tDescartesRegion);
	if(error)
	{
		perror("compute_string_bbox\n");
		return;
	}

	pen_x = tRegion.iLeftUpX + (tRegion.iWidth - tDescartesRegion.iWidth)/2 - tDescartesRegion.iLeftUpX; /*tDescartesRegion.iLeftUpX是相对于字符原点的值*/
	pen_y = tRegion.iLeftUpY + (tRegion.iHeigh - tDescartesRegion.iHeigh)/2 + tDescartesRegion.iLeftUpY; /*tDescartesRegion.iLeftUpY是相对于字符原点的值*/

	while(textname[i])
	{
		/* get bitmap */
		tFontBitmap.iCurOriginX = pen_x;
		tFontBitmap.iCurOriginY = pen_y;
		error = AppGetFontBitmap(&tFontBitmap,textname[i]);//获得位图数据，并更新下一个字符位置信息
		if (error)
		{
			printf("SelectAndInitFont err\n");
			return;
		}

		/* draw on buffer */		
		DrawFontBitmap(&tFontBitmap, color);		

		pen_x = tFontBitmap.iNextOriginX;
		pen_y = tFontBitmap.iNextOriginY;	
		i++;
	}
}

/*描点函数，一个像素点*/
int PutPixel(int x, int y, unsigned int dwColor)
{
	unsigned char *pen_8 = (unsigned char *)(tFrameBuffer_data.buff+y*line_width+x*pixel_width);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (tFrameBuffer_data.ibpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", tFrameBuffer_data.ibpp);
			return -1;
			break;
		}
	}

	return 0;
}

/*显示字符位图*/
void DrawFontBitmap(pFontBitmap ptFontBitmap,unsigned int dwColor)
{
    int  i, j, p, q;
	int x = ptFontBitmap->tRegion.iLeftUpX;
	int y = ptFontBitmap->tRegion.iLeftUpY;
    int  x_max = x + ptFontBitmap->tRegion.iWidth;
    int  y_max = y + ptFontBitmap->tRegion.iHeigh;//LCD坐标系
	int width = ptFontBitmap->tRegion.iWidth;
	unsigned char* buffer = ptFontBitmap->bitmapBuff;

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0      || j < 0       ||
                i >= tFrameBuffer_data.ix || j >= tFrameBuffer_data.iy )
            continue;
            if (buffer[q * width + p])
	            PutPixel(i, j, dwColor);
        }
    }	
}

/*底层信息接口函数，获取底层设备信息到中间层*/
void DisplayDeviceRegister(PDispManager ptDispManager)
{
    ptDispManager->Pnext = temp_PDispManager;
    temp_PDispManager = ptDispManager;
}


/*提供给应用层的接口函数，应用层通过传入设备名选择对应的结构体*/
int select_DeviceName(unsigned char *DeviceName)
{
    PDispManager tmp = temp_PDispManager;
    while(tmp)
    {
        if(strcmp((char *)tmp->name,(char *)DeviceName) == 0)
        {
            Now_PDispManager = tmp;
            return 0;
        }
        tmp = tmp->Pnext;
    }

    return -1;
}

/*提供给应用层的接口函数，初始化屏幕，获取屏幕信息*/
int Get_Disp_Data(void)
{
    int ret = 0;
    Now_PDispManager->DeviceInit();
    if(ret)
    {
        printf("DeviceInit err\n");
        return -1;
    }

    Now_PDispManager->GetFramebuff(&tFrameBuffer_data);
    if(ret)
    {
        printf("GetFramebuff err\n");
        return -1;
    }
	line_width  = tFrameBuffer_data.ix * tFrameBuffer_data.ibpp/8;
	pixel_width = tFrameBuffer_data.ibpp/8;

    return 0;

}

/*提供应用层，关闭打开的文件*/
void Display_Exit(void)
{
    Now_PDispManager->DeviceExit();
}

/*，应用层调用，Flash刷写，未使用*/
int FlushDisplayRegion(PRegion ptRegion, PFrameBuffer_data ptDispBuff)
{
	
	return  Now_PDispManager->FullFlash(ptRegion,ptDispBuff);
}

/*应用层调用,将屏幕信息封装成函数提供给应用层*/
PFrameBuffer_data GetDisplayBuffer(void)
{
	return &tFrameBuffer_data;
}

/*应用层调用，将底层数据传到中间层*/
void DisplaySystemRegister(void)
{
	extern void FramebufferInit(void);
    FramebufferInit();
}