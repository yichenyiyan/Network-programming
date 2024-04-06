/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



//server(version1.2) 多进程实现多用户在线、信号处理中断、退出(解决一下端口不能复用问题)

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>
#include<pthread.h>
#include <arpa/inet.h>
#include <sys/wait.h>


#include<iostream>

using namespace std;

#define BUF_MAX_SIZE 256
#define SERVER_IP_ADDR "0.0.0.0"
#define SERVER_LISTEN_PORT 8888
#define STR_INPUT1   "quit"
#define STR_INPUT2   "exit"
#define STR_COMPARE1(v)   (strcmp(v,STR_INPUT1) == 0)
#define STR_COMPARE2(v)   (strcmp(v,STR_INPUT2) == 0)


static int ret = 0;
static int cfd;
static int sockfd;
//static char buffer[BUF_MAX_SIZE];
static struct sockaddr_in server;
static struct sockaddr_in client;
//static socklen_t addrlen = sizeof(struct sockaddr_in);



//const char* server_say_hello = "hello,welcome! I'm server.";
const char* server_say_hello = "hello, my client I'm server\0";


void siginthandler(int num)
{
	//关闭时需要进行的处理
	printf("recv from ctrl + c signal quit server!\n");
	close(sockfd);
	close(cfd);
	exit(1);
	//return;
}


//子进程处理函数
void son_handler(int cfd, struct sockaddr_in client)
{
	int p_ret = 0;
	char buffer[BUF_MAX_SIZE];
	p_ret = send(cfd, server_say_hello, sizeof server_say_hello, 0);
	if (p_ret < 0) {
		perror("first send");
		return;
	}
	else {
		printf("success to say hello\n");
	}
	p_ret = recv(cfd, buffer, sizeof buffer, 0);
	if (p_ret < 0) {
		perror("first recv");
		return;
	}
	else {
		printf("client[>]:%s\n",buffer);
	}
	while (1) {
		p_ret = send(cfd, buffer, sizeof buffer, 0);
		if (p_ret < 0) {
			perror("first send");
			return;
		}
		else {
			printf("resend success!\n");
		}

		memset(buffer, 0, sizeof buffer);
		p_ret = recv(cfd, buffer, sizeof buffer, 0);
		if (p_ret < 0) {
			perror("first recv");
			return;
		}
		else if (p_ret == 0) {
			printf("client disconnect\n");
			return;
		}
		else {
			printf("client[>]:%s\n", buffer);
		}
		if (STR_COMPARE1(buffer) || STR_COMPARE2(buffer)) {
			printf("client disconnect\n");
			return;
		}
	}



	return;
}



int main()
{
	//补抓ctrl+c信号
	signal(SIGINT, siginthandler);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		exit(1);
	}

	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = atoi(SERVER_IP_ADDR);
	server.sin_port = htons(SERVER_LISTEN_PORT);
	socklen_t addrlen = sizeof(struct sockaddr_in);

	//设置端口复用
	int opt = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//绑定端口
	ret = bind(sockfd, (struct sockaddr*)&server, addrlen);
	if (ret < 0) {
		perror("bind");
		close(sockfd);
		exit(1);
	}
	
	//监听端口
	ret = listen(sockfd, 5);
	if (ret < 0) {
		perror("listen");
		close(sockfd);
		exit(1);
	}

	pid_t pid;
	while (1) {
		memset(&client, 0, sizeof client);
		cfd = accept(sockfd, (struct sockaddr*)&client, &addrlen);
		if (cfd < 0) {
			perror("accept");
			//close(sockfd);
			exit(1);
		}
		else {
			printf("client had connected!\nclient ip: %s port: %d server fd: %d\n",
				inet_ntoa(client.sin_addr), ntohs(client.sin_port),cfd);
		}

		pid = fork();
		if (pid == -1) {
			perror("fork");
		}
		else if (pid == 0) {
			//子进程
			printf("I'm child process, my PID is %d.\n", getpid());
			printf("I'm connecting with client %d\n", cfd);
			son_handler(cfd, client);
		}
		else{
			//父进程
			printf("I'm father process, my PID is %d.\nNow,I'm waiting for my son...\n", getpid());
			//wait(&pid);
			close(cfd);
			continue;
		}




	}

	close(sockfd);


	return 0;
}
