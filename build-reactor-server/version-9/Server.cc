/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <functional>

#include "Server.hh"
#include "Socket.hh"
#include "Acceptor.hh"
#include "Connection.hh"
#include "InetAddress.hh"


#define READ_BUFFER 1024


yichen::Server::Server(yichen::EventLoop *_loop) 
    : loop(_loop), acceptor(nullptr) {    
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

yichen::Server::~Server() {
    delete acceptor;
}



void 
yichen::Server::newConnection(Socket* sock) {
    if (sock->m_getFd() != -1) {
        Connection *conn = new Connection(loop, sock);
        std::function<void(int)> cb = std::bind(&yichen::Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[sock->m_getFd()] = conn;
    }
}

void 
yichen::Server::deleteConnection(int sockfd) {
    if (sockfd != -1) {
        auto it = connections.find(sockfd);
        if (it != connections.end()) {
            Connection *conn = connections[sockfd];
            connections.erase(sockfd);
            // close(sockfd);       //正常
            delete conn;         //会Segmant fault
        }
    }
}

