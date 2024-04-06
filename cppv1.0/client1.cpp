/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//client(version 1.0)
#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>


using namespace std;

#define BUF_MAX_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT  8888


static int sockfd;
static char buffer[BUF_MAX_SIZE];
static struct sockaddr_in my_addr;


int main(void)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	socklen_t addrlen = sizeof(struct sockaddr_in);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVER_PORT);
	my_addr.sin_addr.s_addr = atoi(SERVER_IP);


	int ret = connect(sockfd, (sockaddr*)&my_addr, addrlen);
	if (ret < 0) {
		perror("connect");
		close(sockfd);
		exit(1);
	}

	memset(buffer, 0, sizeof buffer);
	ret = recv(sockfd, buffer, sizeof buffer, 0);
	if (ret < 0) {
		perror("recv");
		close(sockfd);
		exit(1);
	}
	printf("information from server:\n");
	printf("%s\n", buffer);

	memset(buffer, 0, sizeof buffer);
	printf("please input your information which need to send to server:\n");
	//scanf("%s", buffer);    
	fgets(buffer, sizeof(buffer), stdin);
	ret = send(sockfd, buffer, sizeof buffer, 0);
	if (ret < 0) {
		perror("send");
		close(sockfd);
		exit(1);
	}
	else {
		printf("success to send\n");
	}

	close(sockfd);




	return 0;
}
