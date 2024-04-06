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
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>


#define ERRIF(expr, msg)    if(expr){\
                               printf("%s\n",(const char*)(msg));\
                               exit(-1);                         \
                            }

#define IP     "0.0.0.0"
#define PORT       8888
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

    ERRIF(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

    while (true) 
    {
        memset(buf, 0, sizeof(buf));
		printf("input your info:\n");
        fgets(buf, sizeof(buf), stdin);
        ssize_t write_bytes = send(sockfd, buf, sizeof(buf), 0);
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = recv(sockfd, buf, sizeof(buf), 0);
        if (read_bytes > 0) {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0) {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1) {
            close(sockfd);
            ERRIF(true, "socket read error");
        }
    }

    close(sockfd);

    return 0;
}

