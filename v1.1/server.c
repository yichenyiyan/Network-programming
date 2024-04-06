/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>

//与一个客户端进行多次通信(echo)，而不是仅通信一回合就断开


#define ERRIF(expr, msg)    if(expr){\
                               printf("%s\n",(const char*)(msg));\
                               exit(-1);                         \
                            }

#define IP    "0.0.0.0"
#define PORT      8888
#define MAXBUFSIZE  128

int main() 
{
    char buf[MAXBUFSIZE] = {0};
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERRIF(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    ERRIF(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    ERRIF(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
    ERRIF(clnt_sockfd == -1, "socket accept error");

    printf("new client fd %d! IP: %s Port: %d\n", 
        clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    while (true) {
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = recv(clnt_sockfd, buf, sizeof(buf), 0);
        if (read_bytes > 0) {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            send(clnt_sockfd, buf, sizeof(buf), 0);
        }
        else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1) {
            close(clnt_sockfd);
            ERRIF(true, "socket read error");
        }
    }

    close(sockfd);
    return 0;
}
