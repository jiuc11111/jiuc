#include  <page_manager.h>
#include <string.h>
#include <stdlib.h>

static pPageAction g_CurPageAction = NULL;   //����ҳ��ṹ��


void MainPageDeviceRegister(pPageAction pageAction)  //App����
{
    pageAction->pNext = g_CurPageAction;
    g_CurPageAction = pageAction;
}

void MainPageSystemRegister(void)
{
   extern void MainRegiter(void);
   MainRegiter();
}


pPageAction AppSelectPage(char* PageName)  //ÿ��ҳ������������
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
