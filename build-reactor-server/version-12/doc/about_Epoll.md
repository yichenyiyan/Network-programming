介绍一下类Epoll:

成员属性:

int epfd_                        epoll实例化的文件描述符
struct epoll_event* events_      epoll_event结构体数组

成员方法:

void UpdateChannel(Channel* ch)                 根据该频道(ch)所管理的文件描述符在epoll的注册情况，更新其文件描述符到epoll实例化对象上进行监听
void DeleteChannel(Channel* ch)                 将该频道所管理的文件描述符从epoll上注销注册(移除监听)
std::vector<Channel *> Poll(int timeout = -1)   事件循环的源头(其实就是epoll_wait,只不过将其返回的epoll_event数组转化成Channel*传递
                                                    给EventLoop::loop)

构造函数:

Epoll()    默认构造:创建epoll实例化并对属性epfd进行赋值，创建失败则退出程序，在堆区开辟一个event_epoll数组（默认容量为1024）