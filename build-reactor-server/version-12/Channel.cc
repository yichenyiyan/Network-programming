/**
 * @file Channel.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <sys/epoll.h>
#include <unistd.h>
#include <utility>

#include "Channel.hh"
#include "Socket.hh"
#include "EventLoop.hh"


yichen::Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}

yichen::Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void 
yichen::Channel::HandleEvent() {
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    read_callback_();
  }
  if (ready_events_ & (EPOLLOUT)) {
    write_callback_();
  }
}

void 
yichen::Channel::EnableRead() {
  listen_events_ |= EPOLLIN | EPOLLPRI;
  loop_->UpdateChannel(this);
}

void 
yichen::Channel::UseET() {
  listen_events_ |= EPOLLET;
  loop_->UpdateChannel(this);
}

int yichen::Channel::GetFd() { return fd_; }

uint32_t yichen::Channel::GetListenEvents() { return listen_events_; }

uint32_t yichen::Channel::GetReadyEvents() { return ready_events_; }

bool yichen::Channel::GetInEpoll() { return in_epoll_; }

void yichen::Channel::SetInEpoll(bool in) { in_epoll_ = in; }

void yichen::Channel::SetReadyEvents(uint32_t ev) { ready_events_ = ev; }

void yichen::Channel::SetReadCallback(std::function<void()> const &callback) { read_callback_ = callback; }
