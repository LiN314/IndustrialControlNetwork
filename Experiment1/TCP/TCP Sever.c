//SERVER
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")//引入动态库
#define myPort 6666 /*定义用户连接端*/
#define backLog 10 /*多少等待连接控制*/
#define maxDataSize 100 /* 每次可以接收的最大字节 */
int main()
{
    int sockfd, new_fd; /*定义套接字*/
    struct sockaddr_in myAddr; /*本地地址信息 */
    struct sockaddr_in theirAddr; /*连接者地址信息*/
    int sin_size,numbytes;
    char msg[10],buf[maxDataSize];
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2),&ws); //初始化Windows Socket DLL (版本号,数据结构指针)

    //建立socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)// IPv4 TCP 一般为0
    {
        //如果建立socket失败，退出程序
        printf("socket error\n");
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

    //bind本机的MYPORT端口
    myAddr.sin_family = AF_INET; /* 协议类型是INET */
    myAddr.sin_port = htons(myPort); /* 绑定MYPORT端口*/
    myAddr.sin_addr.s_addr = INADDR_ANY; //0.0.0.0 即在所有IP地址进行侦听
    if (bind(sockfd, (struct sockaddr *)&myAddr, sizeof(struct sockaddr))== -1) //绑定IP地址、端口号等
    {
        //bind失败，退出程序
        printf("bind error\n");
        closesocket(sockfd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }

    //listen，监听端口
    if (listen(sockfd, backLog) == -1)
    {
        //listen失败，退出程序
        printf("listen error\n");
        closesocket(sockfd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    printf("listen...\n");

    //等待客户端连接
    sin_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&theirAddr, &sin_size)) == -1)
    {
        printf("accept error\n");
        closesocket(sockfd);
        exit(1);
    }
    printf("\naccept!\n");

    while(1) {
    if((numbytes=recv(new_fd, buf, maxDataSize, 0)) == -1) continue;
    if(!strcmp(buf,"bye"))//若客户端发送bye,则关闭
    {
        //成功，关闭套接字
        closesocket(sockfd);
        closesocket(new_fd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        return 0;
    }
    printf("%s %d\n",buf,strlen(buf));
    sprintf(msg,"%d\n",strlen(buf));
    if (send(new_fd,msg,maxDataSize, 0) == -1)
    {
        printf("send error\n");
        closesocket(sockfd);
        closesocket(new_fd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        return 0;
    }
}
}
