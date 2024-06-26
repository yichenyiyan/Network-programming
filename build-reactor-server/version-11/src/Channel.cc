/**
 * @file Channel.cc
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#include <unistd.h>

#include "../include/Channel.hh"
#include "../include/Epoll.hh"
#include "../include/EventLoop.hh"

yichen::Channel::Channel(yichen::EventLoop* _loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), usePthreadPool(false) {}

yichen::Channel::~Channel() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}



void 
yichen::Channel::handleEvent() {
    if (ready & (EPOLLIN | EPOLLPRI)) 
        readCallback();
    
    if (ready & (EPOLLOUT)) 
        writeCallback();
    

}

void 
yichen::Channel::enableRead() {
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void 
yichen::Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
}
int 
yichen::Channel::m_getFd() {
    return fd;
}

uint32_t 
yichen::Channel::m_getEvents() {
    return events;
}

uint32_t 
yichen::Channel::m_getReady() {
    return ready;
}

bool 
yichen::Channel::m_getInEpoll() {
    return inEpoll;
}

void 
yichen::Channel::m_setInEpoll(bool _in) {
    inEpoll = _in;
}

void 
yichen::Channel::m_setReady(uint32_t _ev) {
    ready = _ev;
}

void 
yichen::Channel::setReadCallback(std::function<void()> _cb) {
    readCallback = _cb;
}


void 
yichen::Channel::setUseThreadPool(bool use) {
    usePthreadPool = use;
}