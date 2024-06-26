/**
 * @file FileName.cpp
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */


#pragma once

#include <functional>
#include <sys/epoll.h>

namespace yichen 
{
    class EventLoop;

    class Channel {
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
         
        int fd;
        bool inEpoll;
        uint32_t events;
        uint32_t ready;
        EventLoop* loop;
        bool usePthreadPool;
        std::function<void()> readCallback;
        std::function<void()> writeCallback;
        Channel(const Channel&) = delete;
        Channel& operator=(const Channel&) = delete;
        

    public:
        Channel(EventLoop* _loop, int _fd);
        ~Channel();

    public:
        void useET();
        void handleEvent();
        void enableRead();

        int m_getFd();
        uint32_t m_getEvents();
        uint32_t m_getReady();
        bool m_getInEpoll();
        void m_setInEpoll(bool _in = true);

        void m_setReady(uint32_t);
        void setReadCallback(std::function<void()>);
        void setUseThreadPool(bool use = true);

    };
}
