/**
 * @file Acceptor.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include "../include/Acceptor.hh"
#include "../include/Socket.hh"
#include "../include/InetAddress.hh"
#include "../include/Channel.hh"
#include "../include/Server.hh"

yichen::Acceptor::Acceptor(EventLoop* _loop) 
    : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
    sock = new Socket();
    InetAddress* addr = new InetAddress("0.0.0.0", 9999);
    sock->m_bind(addr);
    sock->m_listen(); 
    //sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->m_getFd());
    std::function<void()> cb = std::bind(&yichen::Acceptor::acceptConnection, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    acceptChannel->setUseThreadPool(false);
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
