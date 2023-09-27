#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>



#include "display_manager.h"

static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;


/*打开文件，读取屏幕信息*/
static int FramebufferDeviceInit(void)
{
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;   //获得x方向字节数
	pixel_width = var.bits_per_pixel / 8;    //获得一个像素字节数
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;  //屏幕大小
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}  
		/* 清屏: 全部设为白色 */
	memset(fb_base, 0x00, screen_size);
    return 0; 
}

/*关闭文件，退出程序*/
static int FramebufferDeviceExit(void) 
{
	munmap(fb_base, screen_size);
	close(fd_fb);
	return 0;
}

/*获得屏幕信息*/
static int FramebufferGetFramebuff(PFrameBuffer_data PtFrameBuffer_data)
{
    PtFrameBuffer_data->ix = var.xres;
    PtFrameBuffer_data->iy = var.yres;
    PtFrameBuffer_data->ibpp = var.bits_per_pixel;
    PtFrameBuffer_data->buff = fb_base; //屏幕映射基地址
    return 0;
}

/*通过刷写flash方式修改屏幕数据,未使用*/
static int FramebufferFullFlash(PRegion ptRegion, PFrameBuffer_data ptDispBuff)
{
    return 0;
}

/*通用结构体初始化*/
static DispManager g_display_manager = {
    .name = (unsigned char *)"fb",
    .DeviceInit =   FramebufferDeviceInit,
    .DeviceExit =   FramebufferDeviceExit,
    .GetFramebuff = FramebufferGetFramebuff,
    .FullFlash   =   FramebufferFullFlash,
    .Pnext = NULL,
};

/*将数据信息注册到中间层*/
void FramebufferInit(void)
{
     DisplayDeviceRegister(&g_display_manager);
}