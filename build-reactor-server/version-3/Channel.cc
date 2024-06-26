#include "Channel.hh"
#include "Epoll.hh"

yichen::Channel::Channel(Epoll *_ep, int _fd) 
    : ep(_ep), fd(_fd), events(0), revents(0), inEpoll(false) {}

yichen::Channel::~Channel() {}


void 
yichen::Channel::enableReading() {
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}