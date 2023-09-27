#include  <page_manager.h>
#include <string.h>
#include <stdlib.h>

static pPageAction g_CurPageAction = NULL;   //所有页面结构体


void MainPageDeviceRegister(pPageAction pageAction)  //App调用
{
    pageAction->pNext = g_CurPageAction;
    g_CurPageAction = pageAction;
}

void MainPageSystemRegister(void)
{
   extern void MainRegiter(void);
   MainRegiter();
}


pPageAction AppSelectPage(char* PageName)  //每个页面用链表串起来
{
    pPageAction temp = g_CurPageAction;
    while(temp)
    {
        if(strcmp(temp->PageName, PageName) == 0)
            return temp;
        temp = temp->pNext;
    }
    return NULL;
}
