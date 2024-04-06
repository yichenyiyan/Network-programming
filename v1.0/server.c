/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



#include <stdio.h>
#include<unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define IP        "0.0.0.0"
#define PORT           8888
#define MAXBUFSIZE      128
#define LISTENQUEUELEN   20

/*服务器一般流程：

创建套接字socket()
将套接字与服务器网络信息结构体绑定bind()
将套接字设置为监听状态listen()
阻塞等待客户端的连接请求accept()进行通信recv()/send()
关闭套接字close()
*/

int main()
{
    char buf[MAXBUFSIZE] = { 0 };
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, LISTENQUEUELEN);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_len);

    printf("new client fd %d! IP: %s Port: %d\n",
        clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    bzero(buf, sizeof(buf));
    recv(clnt_sockfd, buf, sizeof(buf), 0);
    printf("from client: %s\n", buf);
    bzero(buf, sizeof(buf));
    sprintf(buf, "hello my client!\n");
    send(clnt_sockfd, buf, sizeof(buf), 0);

    close(sockfd);

    return 0;
}
