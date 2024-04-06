/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//client(version1.3) 多线程server对应量身定做客户端
#include<cstdio>
#include<cstdlib>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<signal.h>
#include<string.h>


#define BUF_MAX_SIZE 256
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

static int ret;
static int sockfd;
static int realret;
static char buffer[BUF_MAX_SIZE] = { 0 };
static struct sockaddr_in server;
static socklen_t addrlen = sizeof(struct sockaddr_in);


void signal_handler(int sig_val)
{
	close(sockfd);
	exit(0);

	return;
}



int main(void)
{
	signal(SIGINT, signal_handler);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = atoi(SERVER_IP);
	server.sin_port = ntohs(SERVER_PORT);

	if (connect(sockfd, (struct sockaddr*)&server, addrlen) < 0) {
		perror("connect");
		close(sockfd);
		exit(1);
	}

	printf("==========connect success!===========\n");
	bzero(buffer, sizeof buffer);
	if (recv(sockfd, buffer, sizeof buffer, 0) < 0) {
		perror("first recv");
		close(sockfd);
		exit(1);
	}
	printf("server[>]:%s\n", buffer);

	while (1) {
		bzero(buffer, sizeof buffer);
		printf("input your information:\n");
		fgets(buffer, sizeof buffer, stdin);
		
		realret = strlen(buffer);
		buffer[realret - 1] = '\0';
		if (send(sockfd, buffer, realret, 0) < 0) {
			perror("send");
			continue;
		}
		bzero(buffer, sizeof buffer);
		if ((ret = recv(sockfd, buffer, sizeof buffer, 0)) < 0) {
			perror("recv");
			continue;
		}
		if (ret == 0) {
			printf("disconnect with server\n");
			break;
		}
		printf("server[size:%d->]:%s\n",(int)strlen(buffer), buffer);
		
	}

	close(sockfd);

	return 0;
}
