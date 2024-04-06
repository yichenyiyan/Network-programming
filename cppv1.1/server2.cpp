/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//server(version1.1) 循环处理消息的接收和发送
#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>


using namespace std;


#define BUF_MAX_SZIE 256
#define SERVER_IP    "0.0.0.0"
#define SERVER_LISTEN_PORT 8181


static int ret = 0;
static int cfd;
static int sockfd;
static socklen_t addrlen;
static char buffer[BUF_MAX_SZIE];
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;




int main()
{
	//初始化socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_LISTEN_PORT);
	server_addr.sin_addr.s_addr = atoi(SERVER_IP);
	addrlen = sizeof(struct sockaddr_in);

	//绑定
	ret = bind(sockfd, (struct sockaddr*)&server_addr, addrlen);
	if (ret < 0) {
		perror("bind");
		exit(1);
	}

	//监听
	ret = listen(sockfd, 3);
	if (ret < 0) {
		perror("listen");
		exit(1);
	}

	printf("start to listen port,server init success!\n");
	memset(&client_addr, 0, addrlen);
	//等待连接
	cfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
	printf("client had connected the ip: %s, port: %d\n",
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	printf("start to chat with the client:\n");
	while (1) {
		memset(buffer, 0, sizeof buffer);
		printf("input your information:\n");
		fgets(buffer, sizeof buffer, stdin);
		ret = send(cfd, buffer, sizeof buffer, 0);
		if (ret < 0) {
			perror("send");
			close(cfd);
			close(sockfd);
			exit(1);
		}
		else {
			printf("success to send!\n");
//			exit(1);
		}

		memset(buffer, 0, sizeof buffer);
		ret = recv(cfd, buffer, sizeof buffer, 0);
		if (ret < 0) {
			perror("recv");
			close(cfd);
			close(sockfd);
			exit(1);
		}
		else if (ret == 0) {
			printf("client disconnect!\nserver quit!\n");
			close(cfd);
			close(sockfd);
			exit(1);
		}
		else {
			printf("your client:\n");
			printf("%s\n", buffer);
		}
		
	}

	close(cfd);
	close(sockfd);

	return 0;
}

