/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <functional>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include "Server.hh"
#include "Socket.hh"
#include "InetAddress.hh"
#include "Channel.hh"

#define READ_BUFFER 1024


yichen::Server::Server(yichen::EventLoop *_loop) : loop(_loop) {    
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("0.0.0.0", 8888);
    serv_sock->m_bind(serv_addr);
    serv_sock->m_listen(); 
    serv_sock->setnonblocking();
       
    Channel *servChannel = new Channel(loop, serv_sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Server::newConnection, this, serv_sock);//this 作为第一个参数传递，用于指定成员函数所属的对象
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

yichen::Server::~Server() {}

void 
yichen::Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } 
        else if (bytes_read == -1 && errno == EINTR) {  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } 
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } 
        else if (bytes_read == 0) {  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void 
yichen::Server::newConnection(Socket *serv_sock) {
    InetAddress *clnt_addr = new InetAddress();      //会发生内存泄露！没有delete
    Socket* clnt_sock = new Socket(serv_sock->m_accept(clnt_addr));       //会发生内存泄露！没有delete
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->m_getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel* clntChannel = new Channel(loop, clnt_sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Server::handleReadEvent, this, clnt_sock->m_getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}
