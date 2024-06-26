/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <vector>

#include "EventLoop.hh"
#include "Epoll.hh"
#include "Channel.hh"
#include "ThreadPool.hh"


yichen::EventLoop::EventLoop() 
    : ep(nullptr), threadPool(nullptr), quit(false) {
    ep = new Epoll();
    threadPool = new ThreadPool();
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

void  
yichen::EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}