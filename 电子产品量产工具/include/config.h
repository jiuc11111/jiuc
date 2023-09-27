#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CONFIG_FILE_DIR  "/etc/test_gui/gui.conf"
#define ITEM_BUTTON_MAX 50

/*�����ļ���ť��Ϣ*/
typedef struct ItemButtonInfo_t
{
    int idex;           //����ť��˳����
    char name[20];      //��ť����
    int touchState;     //�ܷ񱻰���
    char command[20];   //���º��Ӧ����磺ִ��ĳ��shell�ű�
}ItemButtonInfo,*pItemButtonInfo;


int ItemParseConfigFile(void);
pItemButtonInfo GetItemCfgByIndex(int index);
pItemButtonInfo GetItemCfgByName(char* name);
int GetItemCfgCount(void);
#endif