#ifndef _COMMON_H_
#define _COMMON_H_

/*ˢдLCD����ṹ��*/
typedef struct Region {
	int iLeftUpX;  
	int iLeftUpY;
	int iWidth;
	int iHeigh;
}Region, *PRegion;

/*һ���ַ���ռ�����Ϣ*/
typedef struct DescartesRegion_t
{
	int iLeftUpX;
	int iLeftUpY;
	int iWidth;
	int iHeigh;	
}DescartesRegion,*pDescartesRegion;

#endif