//SERVER
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32")//引入动态库
#define myPort 6666 /*定义用户连接端*/
#define backLog 10 /*多少等待连接控制*/
#define maxDataSize 100 /* 每次可以接收的最大字节 */
int main()
{
    int srv_sd, cli_sd; /*定义套接字*/
    struct sockaddr_in svrAddr; /*本地地址信息 */
    struct sockaddr_in cliAddr; /*连接者地址信息*/
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int sin_size,numbytes;
    char msg[10],buf[maxDataSize];
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2),&ws); //初始化Windows Socket DLL (版本号,数据结构指针)

    //建立socket
    if ((srv_sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)// IPv4 UDP 一般为0
    {
        //如果建立socket失败，退出程序
        printf("socket error\n");
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

    //bind本机的MYPORT端口
    svrAddr.sin_family = AF_INET; /* 协议类型是INET */
    svrAddr.sin_port = htons(myPort); /* 绑定myPort端口*/
    svrAddr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0 即在所有IP地址进行侦听
    if (bind(srv_sd, (struct sockaddr *)&svrAddr, addrlen)== -1) //绑定IP地址、端口号等
    {
        //bind失败，退出程序
        printf("bind1 error\n");
        closesocket(srv_sd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

    printf("waiting...\n");
    //接收客户端的链接
    if ((numbytes=recvfrom(srv_sd, buf, maxDataSize, 0, (struct sockaddr *)&cliAddr, &addrlen)) == -1)
    {
        //recvfrom失败,退出
        printf("recvfrom error\n");
        closesocket(srv_sd);
        closesocket(cli_sd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    printf("client IPAddr = %s, Port = %d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

    //创建与客户端数据交互的套接字
    if ((cli_sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)// IPv4 UDP 一般为0
    {
        //如果建立socket失败，退出程序
        printf("socket error\n");
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

     //为新套接字绑定地址信息
    svrAddr.sin_family = AF_INET; /* 协议类型是INET */
    svrAddr.sin_port = htons(2222);//分配一个连接端口
    svrAddr.sin_addr.s_addr = INADDR_ANY;//不指定连接地址
    if (bind(cli_sd, (struct sockaddr *)&svrAddr, addrlen)== -1) //绑定IP地址、端口号等
    {
        //bind失败，退出程序
        printf("bind2 error\n");
        printf("error code: %d\n",WSAGetLastError);
        closesocket(srv_sd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }


     //为新套接字指定目的地址，接下来的数据交互将可以采用recv()和send()
     if (connect(cli_sd, (struct sockaddr *)&cliAddr,addrlen) == -1)
    {
        //如果连接失败，退出程序
        printf("connet error\n");
        closesocket(srv_sd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

    //回复数据
    printf("%s %d\n",buf,strlen(buf));
    sprintf(msg,"%d\n",strlen(buf));
    if (send(cli_sd,msg,maxDataSize, 0) == -1)
    {
        printf("send error\n");
        closesocket(srv_sd);
        closesocket(cli_sd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        return 0;
    }
    printf("sent!\n");

    while(1)
    {
        if((numbytes=recv(cli_sd, buf, maxDataSize, 0)) == -1) continue;
        if(!strcmp(buf,"bye"))//若客户端发送bye,则关闭
        {
            //成功，关闭套接字
            closesocket(srv_sd);
            closesocket(cli_sd);
            WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
            return 0;
        }
        printf("%s %d\n",buf,strlen(buf));
        sprintf(msg,"%d\n",strlen(buf));
        if (send(cli_sd,msg,maxDataSize, 0) == -1)
        {
            printf("send error\n");
            closesocket(srv_sd);
            closesocket(cli_sd);
            WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
            return 0;
        }
    }
    closesocket(cli_sd);
    return 0;
}
