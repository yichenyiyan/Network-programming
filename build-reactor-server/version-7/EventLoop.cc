/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include "EventLoop.hh"
#include "Epoll.hh"
#include "Channel.hh"
#include <vector>

yichen::EventLoop::EventLoop() 
    : ep(nullptr), quit(false) {
    ep = new Epoll();
}

yichen::EventLoop::~EventLoop() {
    delete ep;
}


void 
yichen::EventLoop::loop() {
    while (!quit) {
    std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void 
yichen::EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}