/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/




#include<cstdio>
#include<cstdlib>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>


#define BUF_MAX_SIZE 1024

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



int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("usage:%s  IP  PORT\n", argv[0]);
		exit(-1);
	}

	signal(SIGINT, signal_handler);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = atoi(argv[1]);
	server.sin_port = ntohs(atoi(argv[2]));

	if (connect(sockfd, (struct sockaddr*)&server, addrlen) < 0) {
		perror("connect");
		close(sockfd);
		exit(1);
	}

	
	bzero(buffer, sizeof buffer);

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
		printf("server[size:%d->]:%s\n", (int)strlen(buffer), buffer);

	}

	close(sockfd);

	return 0;
}
