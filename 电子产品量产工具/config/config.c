#include <config.h>
#include <stdio.h>
#include <string.h>

ItemButtonInfo g_tItemButtonInfo[ITEM_BUTTON_MAX] ={0};  //����������ļ�����İ�ť��Ϣ
unsigned int g_tItemButtonCount = 0;     //��ť����

int ItemParseConfigFile(void)
{
    FILE * fd;
    char buffer[100];
    char *p;

    fd = fopen(CONFIG_FILE_DIR, "r");
	if (!fd)
	{
        printf("%d\n",fd);
        printf("can not open cfg file %s\n", CONFIG_FILE_DIR);
		return -1;
	}

    while(fgets(buffer, 100, fd))
    {
        //buffer[99] = '\0';
        p = buffer;
        /*�����ո��tab��*/
        while(*p == ' ' || *p == '\t')
            p++;

        /*����ע����#*/
        if(*p == '#')
            continue;
        /*��ÿ�����ݴ���g_tItemButtonInfo�ṹ������*/
        g_tItemButtonInfo[g_tItemButtonCount].command[0] = '\0';
        g_tItemButtonInfo[g_tItemButtonCount].idex = g_tItemButtonCount;
        sscanf(p, "%s %d %s", g_tItemButtonInfo[g_tItemButtonCount].name,&g_tItemButtonInfo[g_tItemButtonCount].touchState, \
                            g_tItemButtonInfo[g_tItemButtonCount].command);

        g_tItemButtonCount++;
    }
    return 0;
}

/*���ݱ��˳��õ���ť��Ϣ*/
pItemButtonInfo GetItemCfgByIndex(int index)
{
    return &g_tItemButtonInfo[index];
}

/*�������ֵõ���ť��Ϣ*/
pItemButtonInfo GetItemCfgByName(char* name)
{
    int i;
    for(i = 0; i < g_tItemButtonCount; i++)
    {
        if(strcmp(name, g_tItemButtonInfo[i].name) == 0)
            return &g_tItemButtonInfo[i];
    }

    return NULL;
}

/*���ذ�ť����*/
int GetItemCfgCount(void)
{
    return g_tItemButtonCount;
}