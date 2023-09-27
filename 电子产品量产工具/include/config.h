#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CONFIG_FILE_DIR  "/etc/test_gui/gui.conf"
#define ITEM_BUTTON_MAX 50

/*配置文件按钮信息*/
typedef struct ItemButtonInfo_t
{
    int idex;           //将按钮按顺序编号
    char name[20];      //按钮名字
    int touchState;     //能否被按下
    char command[20];   //按下后对应命令，如：执行某个shell脚本
}ItemButtonInfo,*pItemButtonInfo;


int ItemParseConfigFile(void);
pItemButtonInfo GetItemCfgByIndex(int index);
pItemButtonInfo GetItemCfgByName(char* name);
int GetItemCfgCount(void);
#endif