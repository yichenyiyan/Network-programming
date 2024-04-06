/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//server(version 1.0)
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<signal.h>

using namespace std;

#define BUF_MAX_SIZE 1024
#define LISTEN_PORT  8888
#define IP_ADDR  "0.0.0.0"


static char buffer[BUF_MAX_SIZE];
static int sockfd = 0;
static int cfd = 0;
static socklen_t addr_len;
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;


int main(void)
{
    //signal();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(-1);
    }


    memset(&server_addr, 0, sizeof server_addr);
    addr_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = atoi(IP_ADDR);

    int ret = bind(sockfd,(struct sockaddr*)&server_addr, addr_len);
    if (ret < 0) {
        perror("bind");
        exit(-1);
    }

    ret = listen(sockfd, 3);//最新理解10为全连接队列的长度
    if (ret < 0) {
        perror("listen");
        close(sockfd);
        exit(-1);
    }
    
    memset(&client_addr, 0, addr_len);
    memset(buffer, 0, sizeof(buffer));
    int cfd = accept(sockfd, (sockaddr*)&client_addr, &addr_len);
    printf("cilent ip:%s port:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("connect success !\nplease input your information that send to cilent:\n");
    //scanf("%s", buffer);
    fgets(buffer, sizeof(buffer), stdin);
    ret = send(cfd, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("send");
        close(cfd);
        close(sockfd);
        exit(1);
    }
    else if (ret == 0) {
        printf("no send data!\n");
    }
    else {
        printf("had send %d bytes data to cilent!\n",ret);
    }
    memset(buffer, 0, sizeof(buffer));
    ret = recv(cfd, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("recv");
        close(cfd);
        close(sockfd);
        exit(1);
    }
    else if (ret == 0) {
        printf("client had leave!\n");

    }
    else {
        printf("the information from client:\n%s\n", buffer);
    }

    close(cfd);
    close(sockfd);

    return 0;
}
