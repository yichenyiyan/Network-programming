/**
 * @file EventLoop.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include <vector>

#include "../include/EventLoop.hh"
#include "../include/Epoll.hh"
#include "../include/Channel.hh"
#include "../include/ThreadPool.hh"


yichen::EventLoop::EventLoop() 
    : ep(nullptr), quit(false) {
    ep = new Epoll();
}

yichen::EventLoop::~EventLoop() {
    delete ep;
}


void 
yichen::EventLoop::loop() {
    while(!quit){
    std::vector<Channel*> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
    }
}

void 
yichen::EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}

