/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/

#pragma once

#include <sys/epoll.h>

namespace yichen 
{
    class Epoll;

    class Channel{
    private:
        Epoll *ep;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
    public:
        Channel(Epoll *_ep, int _fd);
        ~Channel();

    public:
        void enableReading();
        // inline 会失效，可能是因为updateChannel
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
