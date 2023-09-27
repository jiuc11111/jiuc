#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

#include "common.h"
#include "font_manager.h"
/*触摸屏信息结构体*/
typedef struct FrameBuffer_data_t
{
  unsigned  int ix;          //x方向分辨率
  unsigned  int iy;          //y方向分辨率
  unsigned  int ibpp;        //一个像素值
  unsigned  char *buff;     //保存映射基地址
}FrameBuffer_data, *PFrameBuffer_data;

/*抽象出的通用结构体*/
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

