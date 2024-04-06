/******************************
*  ���ߣ��õ�����ŵ���(CSDN) *
*  Github:��С���С�Ե�      *
*       yichenyan  QAQ        *
* ���ѧϰӦ���Ǽ�����Ȥ��  *
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

/*������һ�����̣�

�����׽���socket()
���׽����������������Ϣ�ṹ���bind()
���׽�������Ϊ����״̬listen()
�����ȴ��ͻ��˵���������accept()����ͨ��recv()/send()
�ر��׽���close()
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
