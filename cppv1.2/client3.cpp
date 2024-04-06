/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//client(version1.2) 退出功能（输入exit、quit时与服务器断开连接）
#include<cstdio>
#include<stdlib.h>
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>


using namespace std;

#define BUF_MAX_SIZE 256
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT  8888
#define STR_INPUT1   "quit"
#define STR_INPUT2   "exit"
#define STR_COMPARE1(v)   (strcmp(v,STR_INPUT1) == 0)
#define STR_COMPARE2(v)   (strcmp(v,STR_INPUT2) == 0)


static int sockfd;
static char buffer[BUF_MAX_SIZE];
static struct sockaddr_in my_addr;

void siginthandler(int num)
{
	//关闭时需要进行的处理
	printf("recv from ctrl + c signal quit server!\n");
	close(sockfd);
	exit(1);
}

int main(void)
{
	signal(SIGINT, siginthandler);

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

	printf("connect success!\n");
	while (1) {
		memset(buffer, 0, sizeof buffer);
		ret = recv(sockfd, buffer, sizeof buffer, 0);
		if (ret < 0) {
			perror("recv");
			close(sockfd);
			exit(1);
		}
		else if (ret == 0) {
			printf("disconnect!\n");
			close(sockfd);
			exit(1);
		}
		else {
			printf("from server:\n%s\n", buffer);
		}

		memset(buffer, 0, sizeof buffer);
		printf("input your information:\n");
		fgets(buffer, sizeof buffer, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		if (STR_COMPARE1(buffer) || STR_COMPARE2(buffer)) {
			send(sockfd, "exit", sizeof "exit", 0);
			break;
		}
		ret = send(sockfd, buffer, sizeof buffer, 0);
		if (ret < 0) {
			perror("send");
			close(sockfd);
			exit(1);
		}
		else {
			printf("success to send\n");
		}
	}

	close(sockfd);




	return 0;
}


