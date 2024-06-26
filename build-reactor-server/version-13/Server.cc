/**
 * @file Server.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <unistd.h>
#include <functional>

#include "Server.hh"
#include "Socket.hh"
#include "Acceptor.hh"
#include "Exception.hh"
#include "EventLoop.hh"
#include "ThreadPool.hh"
#include "Connection.hh"

yichen::Server::Server(EventLoop* loop) 
    : main_reactor_(loop), acceptor_(nullptr), thread_pool_(nullptr) {
  if(main_reactor_ == nullptr){
    throw Exception(ExceptionType::INVALID, "main reactor can't be nullptr!");
  }
  acceptor_ = new Acceptor(main_reactor_);
  std::function<void(Socket*)> cb = std::bind(&yichen::Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = static_cast<int>(std::thread::hardware_concurrency());
  thread_pool_ = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    sub_reactors_.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
    thread_pool_->Add(std::move(sub_loop));
  }
}

yichen::Server::~Server() {
  for(auto el : sub_reactors_){
    delete el;
  }
  delete acceptor_;
  delete thread_pool_;
}

void 
yichen::Server::NewConnection(Socket* sock) {
  if (sock->GetFd() == -1) {
    throw Exception(ExceptionType::INVALID_SOCKET, "New Connection error, invalid client socket!");
  }
  uint64_t random = sock->GetFd() % sub_reactors_.size();
  Connection* conn = new Connection(sub_reactors_[random], sock);
  std::function<void(Socket*)> cb = std::bind(&yichen::Server::DeleteConnection, this, std::placeholders::_1);
  conn->SetDeleteConnectionCallback(cb);
  conn->SetOnMessageCallback(on_message_callback_);
  connections_[sock->GetFd()] = conn;
  if (new_connect_callback_)
    new_connect_callback_(conn);
}

void 
yichen::Server::DeleteConnection(Socket *sock) {
  int sockfd = sock->GetFd();
  auto it = connections_.find(sockfd);
  if (it != connections_.end()) {
    Connection* conn = connections_[sockfd];
    connections_.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void yichen::Server::OnConnect(std::function<void(Connection*)> fn) { on_connect_callback_ = std::move(fn); }

void yichen::Server::OnMessage(std::function<void(Connection*)> fn) { on_message_callback_ = std::move(fn); }

void yichen::Server::NewConnect(std::function<void(Connection*)> fn) { new_connect_callback_ = std::move(fn); }
