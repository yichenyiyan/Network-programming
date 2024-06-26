/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <functional>
#include <sys/epoll.h>

namespace yichen 
{
    class EventLoop;

    class Channel{
    private:
        EventLoop* loop;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
        std::function<void()> callback;

    public:
        Channel(EventLoop* _loop, int _fd);
        ~Channel();

    public:
        inline void handleEvent() {
            callback();
        }
        
        inline void setCallback(std::function<void()> _cb) {
            callback = _cb;
        }

        void enableReading();
        // 这里使用内联函数会失效，可能是因为updateChannel
        // inline void enableReading() {
        //     events = EPOLLIN | EPOLLET;
        //     ep->updateChannel(this);
        // }


        inline int m_getFd() {
            return fd;
        }

        inline uint32_t m_getEvents() {
            return events;
        }

        inline uint32_t m_getRevents() {
            return revents;
        }

        inline bool m_getInEpoll() {
            return inEpoll;
        }

        inline void m_setInEpoll() {
            inEpoll = true;
        }

        inline void m_setRevents(uint32_t _ev) {
            revents = _ev;
        }
    };
}
