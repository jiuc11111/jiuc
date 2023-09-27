#ifndef _COMMON_H_
#define _COMMON_H_

/*刷写LCD区域结构体*/
typedef struct Region {
	int iLeftUpX;  
	int iLeftUpY;
	int iWidth;
	int iHeigh;
}Region, *PRegion;

/*一行字符所占外框信息*/
typedef struct DescartesRegion_t
{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iHeigh;	
}DescartesRegion,*pDescartesRegion;

#endif