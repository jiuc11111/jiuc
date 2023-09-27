#include <stdio.h>
#include <string.h>

#include "display_manager.h"

static PDispManager temp_PDispManager = NULL;  //��ŵײ㴫���ͨ�ýṹ����Ϣ
static PDispManager Now_PDispManager = NULL;  //��ǰ�豸��Ϣ
static FrameBuffer_data tFrameBuffer_data ;   //��������Ϣ
static int line_width;       //���ֽ���
static int pixel_width;      //һ�����ض��ٸ��ֽ�


/*��ָ���������ָ����ɫ*/
void DrawBaseColor(Region tRegion,unsigned int color)
{
    int  i, j, p, q;
	int x = tRegion.iLeftUpX;
	int y = tRegion.iLeftUpY;
    int  x_max = x + tRegion.iWidth; //LCD����ϵ
    int  y_max = y + tRegion.iHeigh;//LCD����ϵ
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
	            PutPixel(i, j, color);
        }
    }	
}


/*��ָ�����������ʾָ����ɫ����*/
void DrawText(char* textname, Region tRegion ,unsigned int color)
{
	int i = 0;
	int pen_x,pen_y;  //LCD����ϵԭ��
	int error;
	FontBitmap tFontBitmap;   
	DescartesRegion tDescartesRegion;  //�����������߽���Ϣ

	/*�����һ���������*/
	error = compute_string_bbox(textname, &tDescartesRegion);
	if(error)
	{
		perror("compute_string_bbox\n");
		return;
	}

	pen_x = tRegion.iLeftUpX + (tRegion.iWidth - tDescartesRegion.iWidth)/2 - tDescartesRegion.iLeftUpX; /*tDescartesRegion.iLeftUpX��������ַ�ԭ���ֵ*/
	pen_y = tRegion.iLeftUpY + (tRegion.iHeigh - tDescartesRegion.iHeigh)/2 + tDescartesRegion.iLeftUpY; /*tDescartesRegion.iLeftUpY��������ַ�ԭ���ֵ*/

	while(textname[i])
	{
		/* get bitmap */
		tFontBitmap.iCurOriginX = pen_x;
		tFontBitmap.iCurOriginY = pen_y;
		error = AppGetFontBitmap(&tFontBitmap,textname[i]);//���λͼ���ݣ���������һ���ַ�λ����Ϣ
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

/*��㺯����һ�����ص�*/
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

/*��ʾ�ַ�λͼ*/
void DrawFontBitmap(pFontBitmap ptFontBitmap,unsigned int dwColor)
{
    int  i, j, p, q;
	int x = ptFontBitmap->tRegion.iLeftUpX;
	int y = ptFontBitmap->tRegion.iLeftUpY;
    int  x_max = x + ptFontBitmap->tRegion.iWidth;
    int  y_max = y + ptFontBitmap->tRegion.iHeigh;//LCD����ϵ
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

/*�ײ���Ϣ�ӿں�������ȡ�ײ��豸��Ϣ���м��*/
void DisplayDeviceRegister(PDispManager ptDispManager)
{
    ptDispManager->Pnext = temp_PDispManager;
    temp_PDispManager = ptDispManager;
}


/*�ṩ��Ӧ�ò�Ľӿں�����Ӧ�ò�ͨ�������豸��ѡ���Ӧ�Ľṹ��*/
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

/*�ṩ��Ӧ�ò�Ľӿں�������ʼ����Ļ����ȡ��Ļ��Ϣ*/
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

/*�ṩӦ�ò㣬�رմ򿪵��ļ�*/
void Display_Exit(void)
{
    Now_PDispManager->DeviceExit();
}

/*��Ӧ�ò���ã�Flashˢд��δʹ��*/
int FlushDisplayRegion(PRegion ptRegion, PFrameBuffer_data ptDispBuff)
{
	
	return  Now_PDispManager->FullFlash(ptRegion,ptDispBuff);
}

/*Ӧ�ò����,����Ļ��Ϣ��װ�ɺ����ṩ��Ӧ�ò�*/
PFrameBuffer_data GetDisplayBuffer(void)
{
	return &tFrameBuffer_data;
}

/*Ӧ�ò���ã����ײ����ݴ����м��*/
void DisplaySystemRegister(void)
{
	extern void FramebufferInit(void);
    FramebufferInit();
}