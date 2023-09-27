#include <config.h>
#include <stdio.h>
#include <string.h>

ItemButtonInfo g_tItemButtonInfo[ITEM_BUTTON_MAX] ={0};  //保存从配置文件读入的按钮信息
unsigned int g_tItemButtonCount = 0;     //按钮个数

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
        /*跳过空格和tab键*/
        while(*p == ' ' || *p == '\t')
            p++;

        /*跳过注释行#*/
        if(*p == '#')
            continue;
        /*将每行数据存入g_tItemButtonInfo结构体数组*/
        g_tItemButtonInfo[g_tItemButtonCount].command[0] = '\0';
        g_tItemButtonInfo[g_tItemButtonCount].idex = g_tItemButtonCount;
        sscanf(p, "%s %d %s", g_tItemButtonInfo[g_tItemButtonCount].name,&g_tItemButtonInfo[g_tItemButtonCount].touchState, \
                            g_tItemButtonInfo[g_tItemButtonCount].command);

        g_tItemButtonCount++;
    }
    return 0;
}

/*根据标号顺序得到按钮信息*/
pItemButtonInfo GetItemCfgByIndex(int index)
{
    return &g_tItemButtonInfo[index];
}

/*根据名字得到按钮信息*/
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

/*返回按钮个数*/
int GetItemCfgCount(void)
{
    return g_tItemButtonCount;
}