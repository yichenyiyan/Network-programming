/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/


#pragma once

namespace yichen
{
    class Epoll;
    class Channel;
    class EventLoop {
    private:
        Epoll *ep;
        bool quit;

    public:
        EventLoop();
        ~EventLoop();

        //事件循环
        void loop();
        void updateChannel(Channel*);
    };
}