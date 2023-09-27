#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H


#define GYO_X 5
#define GYO_Y 5 

/*ҳ����Ϣ�ṹ��*/
typedef struct PageAction_t
{
    char* PageName;  //��������ߴμ�����
    void (*PageHandler)(void* page);  //�����Ӧ�����к���
    struct PageAction_t* pNext;   //ָ����һ������
}PageAction, *pPageAction;


void MainPageDeviceRegister(pPageAction pageAction);
void MainPageSystemRegister(void);
pPageAction AppSelectPage(char* PageName);

#endif

