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
    addr = new InetAddress("0.0.0.0", 8888);
    sock->m_bind(addr);
    sock->m_listen(); 
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

yichen::Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete acceptChannel;
}


