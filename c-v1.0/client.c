/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#define IP  "0.0.0.0"
#define PORT  8888
#define MAXBUFSIZE 128


//客户端一般流程：
//创建套接字socket
//发送客户端连接请求connect()
//进行通信recv()/send()
//关闭套接字close()


int main()
{
    char buf[MAXBUFSIZE] = { 0 };
    //创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));//结构体清零
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);


    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    bzero(buf, sizeof(buf));
    sprintf(buf, "hello server\n");
    send(sockfd, buf, sizeof(buf), 0);
    bzero(buf, sizeof(buf));
    recv(sockfd, buf, sizeof(buf), 0);
    printf("from server:%s\n", buf);


    close(sockfd);

    return 0;
}
