/**
 * @file Acceptor.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <utility>

#include "Socket.hh"
#include "Channel.hh"
#include "Acceptor.hh"

yichen::Acceptor::Acceptor(EventLoop* loop) 
    : loop_(loop), sock_(nullptr), channel_(nullptr) {
  sock_ = new Socket();
  InetAddress *addr = new InetAddress("0.0.0.0", 9999);
  sock_->Bind(addr);
  // sock->setnonblocking(); acceptor使用阻塞式IO比较好
  sock_->Listen();
  channel_ = new Channel(loop_, sock_->GetFd());
  std::function<void()> cb = std::bind(&yichen::Acceptor::AcceptConnection, this);
  channel_->SetReadCallback(cb);
  channel_->EnableRead();
  delete addr;
}

yichen::Acceptor::~Acceptor() {
  delete channel_;
  delete sock_;
}

void 
yichen::Acceptor::AcceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), clnt_addr->GetIp(), clnt_addr->GetPort());
  clnt_sock->SetNonBlocking();  // 新接受到的连接设置为非阻塞式
  new_connection_callback_(clnt_sock);
  delete clnt_addr;
}

void 
yichen::Acceptor::SetNewConnectionCallback(std::function<void(Socket*)> const &callback) {
  new_connection_callback_ = callback;
}
