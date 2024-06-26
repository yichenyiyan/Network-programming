/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "Socket.hh"
#include "Channel.hh"
#include "Connection.hh"

#define READ_BUFFER 1024

yichen::Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr){
    channel = new Channel(loop, sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Connection::echo, this, sock->m_getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

yichen::Connection::~Connection(){
    delete channel;
    delete sock;
}

void 
yichen::Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(sock);
            break;
        }
    }
}

