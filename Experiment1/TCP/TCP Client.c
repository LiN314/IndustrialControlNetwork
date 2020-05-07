//CLIENT:
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"Ws2_32")//引入动态库
#define port 6666 /* 客户机连接远程主机的端口 */
#define maxDataSize 100 /* 每次可以接收的最大字节 */
int main()
{
    int sockfd, numbytes;
    char buf[maxDataSize];
    char msg[maxDataSize];
    char *argv="127.0.0.1";//环回测试地址
    //char *argv="192.168.31.124";//多机测试地址
    struct sockaddr_in theirAddr; /* 对方的地址端口信息 */
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2),&ws); //初始化Windows Socket Dll
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // IPv4 TCP
    {
        //如果建立socket失败，退出程序
        printf("socket error\n");
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    //连接对方
    theirAddr.sin_family = AF_INET; /* 协议类型是INET */
    theirAddr.sin_port = htons(port); /* 连接对方PORT端口 */
    theirAddr.sin_addr.s_addr = inet_addr(argv); /* 连接对方的IP */
    if (connect(sockfd, (struct sockaddr *)&theirAddr,sizeof(struct sockaddr)) == -1)
    {
        //如果连接失败，退出程序
        printf("connet error\n");
        closesocket(sockfd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    printf("connected!\n");
    while(1){
    scanf("%s",msg);
    //发送数据
    if (send(sockfd, msg,maxDataSize, 0) == -1)
    {
        printf("send error\n");
        closesocket(sockfd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    //接收数据,并打印出来
    if ((numbytes=recv(sockfd, buf,maxDataSize, 0)) == -1)
    {
        //接收数据失败,退出程序
        printf("recv error\n");
        closesocket(sockfd);
        WSACleanup();//解除与Socket库的绑定并且释放Socket库所占用的系统资源
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("Received: %s",buf); }
    closesocket(sockfd);
    return 0;
}
