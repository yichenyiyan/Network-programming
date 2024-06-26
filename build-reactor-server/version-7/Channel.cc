/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#include <unistd.h>

#include "Channel.hh"
#include "Epoll.hh"
#include "EventLoop.hh"

yichen::Channel::Channel(yichen::EventLoop* _loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false) {}

yichen::Channel::~Channel() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}


void 
yichen::Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}