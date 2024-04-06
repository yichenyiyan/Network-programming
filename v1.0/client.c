/******************************
*  ���ߣ��õ�����ŵ���(CSDN) *
*  Github:��С���С�Ե�      *
*       yichenyan  QAQ        *
* ���ѧϰӦ���Ǽ�����Ȥ��  *
*******************************/



#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#define IP  "0.0.0.0"
#define PORT  8888
#define MAXBUFSIZE 128


//�ͻ���һ�����̣�
//�����׽���socket
//���Ϳͻ�����������connect()
//����ͨ��recv()/send()
//�ر��׽���close()


int main()
{
    char buf[MAXBUFSIZE] = { 0 };
    //����socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));//�ṹ������
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
