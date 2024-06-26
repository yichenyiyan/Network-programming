/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include "Acceptor.hh"
#include "Socket.hh"
#include "InetAddress.hh"
#include "Channel.hh"
#include "Server.hh"

yichen::Acceptor::Acceptor(EventLoop* _loop) 
    : loop(_loop) {
    sock = new Socket();
    InetAddress* addr = new InetAddress("0.0.0.0", 9999);
    sock->m_bind(addr);
    sock->m_listen(); 
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete addr;
}

yichen::Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void 
yichen::Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();      
    Socket *clnt_sock = new Socket(sock->m_accept(clnt_addr));      
    printf("new client fd %d! IP: %s Port: %d\n", 
        clnt_sock->m_getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}
