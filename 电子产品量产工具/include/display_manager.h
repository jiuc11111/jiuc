#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

#include "common.h"
#include "font_manager.h"
/*��������Ϣ�ṹ��*/
typedef struct FrameBuffer_data_t
{
  unsigned  int ix;          //x����ֱ���
  unsigned  int iy;          //y����ֱ���
  unsigned  int ibpp;        //һ������ֵ
  unsigned  char *buff;     //����ӳ�����ַ
}FrameBuffer_data, *PFrameBuffer_data;

/*�������ͨ�ýṹ��*/
typedef struct DispManager_t
{
  unsigned  char *name;
  int (*DeviceInit)(void);
   int (*DeviceExit)(void);
   int (*GetFramebuff)(PFrameBuffer_data PtFrameBuffer_data);
   int (*FullFlash)(PRegion ptRegion, PFrameBuffer_data ptDispBuff);
  struct DispManager_t *Pnext;

}DispManager,*PDispManager;

void DrawText(char* textname, Region tRegion ,unsigned int color);
void DrawBaseColor(Region tRegion,unsigned int color);
void DrawFontBitmap(pFontBitmap ptFontBitmap,unsigned int dwColor);
void DisplayDeviceRegister(PDispManager ptDispManager);
int PutPixel(int x, int y, unsigned int dwColor);
void DisplaySystemRegister(void);
int select_DeviceName(unsigned char *DeviceName);
PFrameBuffer_data GetDisplayBuffer(void);
int Get_Disp_Data(void);
void Display_Exit(void);
#endif

