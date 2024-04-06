/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//server(version1.3) 多线程实现(客户退出功能就不做了)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>


#define BUF_MAX_SIZE		 256
#define SERVER_LISTEN_PORT  8888
#define SERVER_IP_ADDR  "0.0.0.0"
//#define SERVER_SAY_HELLO "hello"

//static char* SERVER_SAY_HELLO = "hello";

//static int cfd;
static int sockfd;
static pthread_t pthread_id;
static struct sockaddr_in server;
static struct sockaddr_in client;
static socklen_t addrlen = sizeof(struct sockaddr_in);


void sigint_handler(int signum)
{
	printf("server is closing...\n");
	usleep(5000);
	close(sockfd);
	exit(1);
	return;
}

void* pthread_handler(void* arg)
{
	int info_bytes = 0;
	//int* _cfd = (int*)arg;
	int _cfd = *(int*)arg;
	char buffer[BUF_MAX_SIZE] = {0};
	//char SERVER_SAY_HELLO[54] = "hello";
	char null_str[24] = "\0";
	sprintf(null_str, "this is your cfd:%d", _cfd);
	//printf("after sprintf SERVER_SY_HELLO is %s\n", SERVER_SAY_HELLO);
	if (send(_cfd, null_str, (size_t)(strlen(null_str)), 0) < 0) {
		perror("error server say hello");
		//printf("client cfd[>]:%d\n",*_cfd);
		close(_cfd);
		pthread_exit(NULL);
	}
	printf("server was success to say hello to client which cfd is %d\n", _cfd);
	while (1) {
		memset(buffer, 0, sizeof buffer);
		if ((info_bytes = recv(_cfd, buffer, sizeof(buffer), 0)) < 0) {
			perror("recv");
			close(_cfd);
			pthread_exit(NULL);
		}
		if (info_bytes == 0) {
			printf("[>]client fd:%d disconnect!\n", _cfd);
			close(_cfd);
			pthread_exit(NULL);
		}
		printf("[<]Form client fd:%d, size:%d[>]:%s\n", _cfd, info_bytes, buffer);
		//buffer[strlen(buffer)] = '\0';
		if ((info_bytes = send(_cfd, buffer, (size_t)(strlen(buffer)), 0)) < 0) {
			perror("send");
			continue;
		}
		printf("[>]Success to send client fd:%d, size:%d\n", _cfd, info_bytes);
	}
	return NULL;
}


int main(void)
{
	signal(SIGINT, sigint_handler);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//设置端口复用
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_LISTEN_PORT);
	server.sin_addr.s_addr = atoi(SERVER_IP_ADDR);

	if (bind(sockfd, (struct sockaddr*)&server, addrlen) < 0) {
		perror("bind");
		close(sockfd);
		exit(1);
	}

	if (listen(sockfd, 5) < 0) {
		perror("listen");
		close(sockfd);
		exit(1);
	}
	printf("server is initing");
	for (int i = 0; i < 6; ++i) {
		sleep(0.5);
		printf(".");
	}
		
	printf("\n");
	printf("==================init success!=======================\n");
	while (1) {
		int cfd;
		memset(&client, 0, sizeof client);
		if ((cfd = accept(sockfd, (struct sockaddr*)&client, &addrlen)) < 0) {
			perror("accept");
			continue;
		}
		//void* cfd_ptr = (void*)(intptr_t)cfd;
		
		//创建默认线程
		//线程id,属性配置（NULL为默认值）,线程处理函数,参数传递
		if (pthread_create(&pthread_id, NULL, pthread_handler, &cfd) < 0) {
			perror("fail to create new pthread");
			continue;
		}
		else {
		//	pthread_join(pthread_id, NULL);
			printf("new client connected!the client[ip]: %s, client[port]: %d\n",
				inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		}

	}


	return 0;
}
