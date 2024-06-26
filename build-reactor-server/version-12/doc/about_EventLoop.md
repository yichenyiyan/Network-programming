介绍一下类EventLoop:


成员属性:

Epoll* epoll_;                      指向Epoll实例化对象的指针
bool quit_;                         事件循环是否退出

成员方法:

void Loop();                        事件循环(quit_为true时停止事件循环)
void UpdateChannel(Channel *ch);    传入一个频道，更新其频道注册信息

构造函数:

EventLoop()                         default:在堆区开辟个Epoll对象并给epoll_属性赋值