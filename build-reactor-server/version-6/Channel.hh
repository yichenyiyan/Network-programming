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
#if 0
struct epoll_event {
    uint32_t events; /* Epoll events */
    epoll_data_t data; /* User data variable */
};
typedef union epoll_data {
    void *ptr;          // 指针类型，可以指向任意类型的数据
    int fd;             // 文件描述符
    uint32_t u32;       // 无符号32位整数
    uint64_t u64;       // 无符号64位整数
} epoll_data_t;

#endif
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
