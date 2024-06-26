/**
 * @file EventLoop.cc
 * @author yichenyan (2513626988@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright （严亦辰）2024
 *
 */

#include <vector>

#include "Epoll.hh"
#include "Channel.hh"
#include "EventLoop.hh"

yichen::EventLoop::EventLoop() : epoll_(nullptr), quit_(false) { epoll_ = new Epoll(); }

yichen::EventLoop::~EventLoop() { delete epoll_; }

void 
yichen::EventLoop::Loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    chs = epoll_->Poll();
    for (auto &ch : chs) {
      ch->HandleEvent();
    }
  }
}

void yichen::EventLoop::UpdateChannel(Channel *ch) { epoll_->UpdateChannel(ch); }