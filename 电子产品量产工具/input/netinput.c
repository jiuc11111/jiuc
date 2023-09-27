#include "input_manager.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



#define SERVER_PORT 8888 //端口号

static 	int iSocketServer;

/*初始化UDP传输*/
static int netInputDeviceInit(void)
{

	struct sockaddr_in tSocketServerAddr;
	int iRet;
	iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}
    return 0;
}


static int netInputDeviceExit(void)
{
    close(iSocketServer);
    return 0;
}

static int netInputDeviceGetInfo(pInputEvent ptInputEvent)
{
    char ucRecvBuf[1024];
    int iRecvLen;
    unsigned int iAddrLen;
    struct sockaddr_in tSocketClientAddr;

    iAddrLen = sizeof(struct sockaddr);
    iRecvLen = recvfrom(iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
    if (iRecvLen > 0)
    {
        ucRecvBuf[iRecvLen] = '\0';
        ptInputEvent->type = INPUT_TYPE_NET;
        gettimeofday(&ptInputEvent->time, NULL);
        strncpy(ptInputEvent->str, ucRecvBuf, 1024);
        return 0;
    }
    else
     return -1;
}
deviceInput g_netInputEvent = 
{
    .name = "netInput",
    .deviceInit = netInputDeviceInit,
    .deviceExit = netInputDeviceExit,
    .deviceGetInfo = netInputDeviceGetInfo,
};

void netInputRegister(void)
{
    InputDeviceRegister(&g_netInputEvent);
}

#if 0

int main(int argc, char **argv)
{
    InputEvent event;
    int ret = 0;

    g_InputEvent.deviceInit();
    while(1)
    {
       ret =  g_InputEvent.deviceGetInfo(&event);
        if(ret)
        {
            printf("netInputDeviceGetInfo err\n");
            return -1;
        }
        else
        {
            printf("type        = %d\n",event.type);
            printf("str         = %s\n",event.str);
        }
    }
    g_InputEvent.deviceExit();
    return 0;
}
#endif