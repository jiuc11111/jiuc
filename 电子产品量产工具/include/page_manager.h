#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H


#define GYO_X 5
#define GYO_Y 5 

/*页面信息结构体*/
typedef struct PageAction_t
{
    char* PageName;  //主界面或者次级界面
    void (*PageHandler)(void* page);  //界面对应主运行函数
    struct PageAction_t* pNext;   //指向下一个界面
}PageAction, *pPageAction;


void MainPageDeviceRegister(pPageAction pageAction);
void MainPageSystemRegister(void);
pPageAction AppSelectPage(char* PageName);

#endif

