/******************************
*  作者：拿得起更放得下(CSDN) *
*  Github:大小姐的小迷弟      *
*       yichenyan  QAQ        *
* 编程学习应该是简单且有趣的  *
*******************************/



#include<iostream>

#include<cstdio>
#include<cstring>
#include<cassert>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<mutex>
#include<thread>
#include<atomic>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>



//对服务器进行更新迭代
//选择使用io多路复用模型epoll


#define BUF_SIZE   1024
#define MAX_EVENTS 1024
#define BOOLERR(formula, msg)   if((formula)){\
								assert(!(formula));\
								printf("%s\n", (const char*)(msg));\
								}

std::mutex mtx;


void sigint_handler(int signum)
{
	printf("ctrl + c: quit\n");
	exit(0);
}



//sokcet初始化
int socketInit(const char* ip, const char* port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	BOOLERR(sockfd < 0, "socket");

	//设置端口复用
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

	struct sockaddr_in server;
	socklen_t addrlen = sizeof(server);
	memset(&server, 0, sizeof server);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = atoi(ip);

	BOOLERR((bind(sockfd, (struct sockaddr*)&server, addrlen) < 0), "bind");
	BOOLERR((listen(sockfd, 5) < 0), "listen");

	return sockfd;
}

//设置文件描述符为非阻塞
void setNoblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void pthreadFunc(int epfd, int clientfd)
{
	{
		std::unique_lock<std::mutex> lck(mtx);
		epoll_ctl(epfd, EPOLL_CTL_DEL, clientfd, NULL);
	}
	
	char buf[BUF_SIZE];
	while (true) {
		bzero(buf, sizeof(buf));
		ssize_t bytes = recv(clientfd, buf, sizeof(buf), 0);
		if (bytes > 0) {
			printf("from fd: %d[>]\n\t:%s\n", clientfd, buf);
			send(clientfd, buf, sizeof(buf), 0);
		}
		else if (bytes == 0) {
			printf("client fd: %d disconnect\n", clientfd);
			
			close(clientfd);
			break;
		}
		else {//返回-1的情况先不做具体区分
			continue;
		}
	}
	return;
}



int main(int argc, char** argv)
{
	signal(SIGINT, sigint_handler);

	if (argc != 3) {
		printf("usage format:%s IP PORT", argv[0]);
		return -1;
	}

	int sockfd = socketInit(argv[1], argv[2]);
	setNoblocking(sockfd);

	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	bzero(&ev, sizeof(ev));
	bzero(&events, sizeof(events));

	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET;

	int epfd = epoll_create(100);
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	while (true) {
		int n = epoll_wait(epfd, events, MAX_EVENTS, 0);
		for (int i = 0; i < n; ++i) {
			if (events[i].data.fd == sockfd) {
				struct sockaddr_in client;
				socklen_t addrlen = sizeof(client);
				bzero(&ev, sizeof(ev));
				bzero(&client, sizeof(client));
				int clientfd = accept(sockfd, (struct sockaddr*)&client, &addrlen);
				printf("new client fd %d! IP: %s Port: %d\n", clientfd, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
				ev.data.fd = clientfd;
				ev.events = EPOLLIN | EPOLLET;
				setNoblocking(clientfd);
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
			}
			else if (events[i].events & EPOLLIN) {//可读事件
				int fd = events[i].data.fd;
				std::thread t(pthreadFunc, epfd, fd);
				t.detach();
				continue;
			}
			else {
				printf("other event!\n");
			}
		}
	}

	close(sockfd);
	close(epfd);

	return 0;
}