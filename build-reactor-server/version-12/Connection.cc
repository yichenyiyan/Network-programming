/**
 * @file Connection.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <cassert>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <iostream>

#include "Buffer.hh"
#include "Socket.hh"
#include "Channel.hh"
#include "Connection.hh"


yichen::Connection::Connection(EventLoop* loop, Socket* sock) 
  : loop_(loop), sock_(sock), channel_(nullptr), read_buffer_(nullptr)
  , send_buffer_(nullptr), state_(State::Invalid) {
  if (loop_ != nullptr) {
    channel_ = new Channel(loop_, sock->GetFd());
    channel_->EnableRead();
    channel_->UseET();
  }
  read_buffer_ = new Buffer();
  send_buffer_ = new Buffer();
  state_ = State::Connected;
}

yichen::Connection::~Connection() {
  if (loop_ != nullptr) {
    delete channel_;
  }
  delete sock_;
  delete read_buffer_;
  delete send_buffer_;
}

void 
yichen::Connection::Read() {
  ASSERT(state_ == State::Connected, "connection state is disconnected!");
  read_buffer_->Clear();
  if (sock_->IsNonBlocking()) {
    ReadNonBlocking();
  } else {
    ReadBlocking();
  }
}


void 
yichen::Connection::Write() {
  ASSERT(state_ == State::Connected, "connection state is disconnected!");
  if (sock_->IsNonBlocking()) {
    WriteNonBlocking();
  } else {
    WriteBlocking();
  }
  send_buffer_->Clear();
}

void 
yichen::Connection::ReadNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[1024];  
    while (true) {   // 使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
      memset(buf, 0, sizeof(buf));
      ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
      if (bytes_read > 0) {
        read_buffer_->Append(buf, bytes_read);
      } 
      else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断、继续读取
        printf("continue reading\n");
        continue;
      } 
      else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
        break;
      } 
      else if (bytes_read == 0) {  // EOF，客户端断开连接
        printf("read EOF, client fd %d disconnected\n", sockfd);
        state_ = State::Closed;
        break;
      } 
      else {
        printf("Other error on client fd %d\n", sockfd);
        state_ = State::Closed;
        break;
      }
    }
}


void 
yichen::Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf, send_buffer_->ToStr(), send_buffer_->Size());
    int data_size = send_buffer_->Size();
    int data_left = data_size;
    while (data_left > 0) {
      ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
      if (bytes_write == -1 && errno == EINTR) {
        printf("continue writing\n");
        continue;
      }
      if (bytes_write == -1 && errno == EAGAIN) {
        break;
      }
      if (bytes_write == -1) {
        printf("Other error on client fd %d\n", sockfd);
        state_ = State::Closed;
        break;
      }
      data_left -= bytes_write;
    }
}

/* only develop for client */
void 
yichen::Connection::ReadBlocking() {
    int sockfd = sock_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buffer_->Append(buf, bytes_read);
    } 
    else if (bytes_read == 0) {
      printf("read EOF, blocking client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
    } 
    else if (bytes_read == -1) {
      printf("Other error on blocking client fd %d\n", sockfd);
      state_ = State::Closed;
    }
}

/* only develop for client */
void 
yichen::Connection::WriteBlocking() {
    // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
    int sockfd = sock_->GetFd();
    ssize_t bytes_write = write(sockfd, send_buffer_->ToStr(), send_buffer_->Size());
    if (bytes_write == -1) {
      printf("Other error on blocking client fd %d\n", sockfd);
      state_ = State::Closed;
    }
}

void yichen::Connection::Close() { delete_connectioin_callback_(sock_); }

yichen::Connection::State yichen::Connection::GetState() { return state_; }

void yichen::Connection::SetSendBuffer(const char* str) { send_buffer_->SetBuf(str); }

yichen::Buffer* yichen::Connection::GetReadBuffer() { return read_buffer_; }

const char* yichen::Connection::ReadBuffer() { return read_buffer_->ToStr(); }

yichen::Buffer* yichen::Connection::GetSendBuffer() { return send_buffer_; }

const char* yichen::Connection::SendBuffer() { return send_buffer_->ToStr(); }

void 
yichen::Connection::SetDeleteConnectionCallback(std::function<void(Socket*)> const &callback) {
  delete_connectioin_callback_ = callback;
}

void 
yichen::Connection::SetOnConnectCallback(std::function<void(Connection*)> const &callback) {
  on_connect_callback_ = callback;
  channel_->SetReadCallback([this]() { on_connect_callback_(this); });
}

void yichen::Connection::GetlineSendBuffer() { send_buffer_->Getline(); }

yichen::Socket* yichen::Connection::GetSocket() { return sock_; }
