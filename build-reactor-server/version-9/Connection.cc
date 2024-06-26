/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "Buffer.hh"
#include "Socket.hh"
#include "Channel.hh"
#include "Connection.hh"

#define TMP_BUFFER 1024

yichen::Connection::Connection(EventLoop *_loop, Socket *_sock) 
    : loop(_loop), sock(_sock), channel(nullptr) 
    , inBuffer(new std::string()), readBuffer(nullptr) {
    channel = new Channel(loop, sock->m_getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&yichen::Connection::echo, this, sock->m_getFd());
    channel->setReadCallback(cb);
    channel->setUseThreadPool(true);
    readBuffer = new Buffer();
}

yichen::Connection::~Connection(){
    delete channel;
    delete sock;
}

void 
yichen::Connection::echo(int sockfd) {
    char buf[1024];     //这个buf大小无所谓
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            readBuffer->append(buf, bytes_read);
        } 
        else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading\n");
            continue;
        } 
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {//非阻塞IO，这个条件表示数据全部读取完毕
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            // errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            send(sockfd);
            readBuffer->clear();
            break;
        } 
        else if (bytes_read == 0) {  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sockfd);           //多线程会有bug
            break;
        } 
        else {
            printf("Connection reset by peer\n");
            deleteConnectionCallback(sockfd);          //会有bug，注释后单线程无bug
            break;
        }
    }
}

void 
yichen::Connection::send(int sockfd) {
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int  data_size = readBuffer->size(); 
    int data_left = data_size; 
    while (data_left > 0) { 
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left); 
        if (bytes_write == -1 && errno == EAGAIN) { 
            break;
        }
        data_left -= bytes_write; 
    }
}
