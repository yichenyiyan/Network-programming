/**
 * @file Socket.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

#include "Socket.hh"



yichen::Socket::Socket() : fd_(-1) {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  DEBUG(fd_ == -1, "socket create error");
}
yichen::Socket::Socket(int fd) : fd_(fd) { DEBUG(fd_ == -1, "socket create error"); }

yichen::Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void 
yichen::Socket::Bind(InetAddress* addr) {
  struct sockaddr_in tmp_addr = addr->GetAddr();
  DEBUG(bind(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket bind error");
}

void yichen::Socket::Listen() { DEBUG(::listen(fd_, SOMAXCONN) == -1, "socket listen error"); }

void yichen::Socket::SetNonBlocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

bool yichen::Socket::IsNonBlocking() { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

int 
yichen::Socket::Accept(InetAddress *addr) {
  // for server socket
  int clnt_sockfd = -1;
  struct sockaddr_in tmp_addr {};
  socklen_t addr_len = sizeof(tmp_addr);
  if (IsNonBlocking()) {
    while (true) {
      clnt_sockfd = accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
      if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        // printf("no connection yet\n");
        continue;
      }
      if (clnt_sockfd == -1) {
        DEBUG(true, "socket accept error");
      } else {
        break;
      }
    }
  } else {
    clnt_sockfd = accept(fd_, (sockaddr *)&tmp_addr, &addr_len);
    DEBUG(clnt_sockfd == -1, "socket accept error");
  }
  addr->SetAddr(tmp_addr);
  return clnt_sockfd;
}

void 
yichen::Socket::Connect(InetAddress *addr) {
  // for client socket
  struct sockaddr_in tmp_addr = addr->GetAddr();
  if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue; /*不断连接直到连接完成，相当于阻塞式*/
      }
      if (ret == -1) {
        DEBUG(true, "socket connect error");
      }
    }
  } else {
    DEBUG(connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
  }
}

void 
yichen::Socket::Connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  Connect(addr);
  delete addr;
}

int yichen::Socket::GetFd() { return fd_; }

yichen::InetAddress::InetAddress() = default;

yichen::InetAddress::InetAddress(const char* ip, uint16_t port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = inet_addr(ip);
  addr_.sin_port = htons(port);
}

void yichen::InetAddress::SetAddr(sockaddr_in addr) { addr_ = addr; }

sockaddr_in yichen::InetAddress::GetAddr() { return addr_; }

const char *yichen::InetAddress::GetIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t yichen::InetAddress::GetPort() { return ntohs(addr_.sin_port); }
