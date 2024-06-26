/**
 * @file Epoll.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

/**
 * @file Epoll.cpp
 * @author 冯岳松 (yuesong-feng@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-04
 *
 * @copyright Copyright (冯岳松) 2022
 *
 */

#include <cstring>
#include <unistd.h>

#include "Epoll.hh"
#include "Channel.hh"

static constexpr int max_events = 1024;

yichen::Epoll::Epoll() {
  epfd_ = epoll_create1(0);
  DEBUG(epfd_ == -1, "epoll create error");
  events_ = new epoll_event[max_events];
  memset(events_, 0, sizeof(*events_) * max_events);
}

yichen::Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }
  delete[] events_;
}

std::vector<yichen::Channel*> 
yichen::Epoll::Poll(int timeout) {
  std::vector<Channel *> active_channels;
  int nfds = epoll_wait(epfd_, events_, max_events, timeout);
  DEBUG(nfds == -1, "epoll wait error");
  for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].data.ptr;
    ch->SetReadyEvents(events_[i].events);
    active_channels.push_back(ch);
  }
  return active_channels;
}

void 
yichen::Epoll::UpdateChannel(Channel *ch) {
  int fd = ch->GetFd();
  struct epoll_event ev {};
  ev.data.ptr = ch;
  ev.events = ch->GetListenEvents();
  if (!ch->GetInEpoll()) {
    DEBUG(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    ch->SetInEpoll();
  } else {
    DEBUG(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

void 
yichen::Epoll::DeleteChannel(Channel *ch) {
  int fd = ch->GetFd();
  DEBUG(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
  ch->SetInEpoll(false);
}

