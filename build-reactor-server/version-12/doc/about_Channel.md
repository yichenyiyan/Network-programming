介绍一下类Channel:

成员属性:

EventLoop* loop_;                           指向该频道所属于的事件循环实例化对象的指针
int fd_;                                    该频道所管理(监听事件就绪)的文件描述符
uint32_t listen_events_;                    该频道为文件描述符fd_在epoll中注册的特定事件类型
uint32_t ready_events_;                     该频道就绪的事件类型
bool in_epoll_;                             是否在epoll中
std::function<void()> read_callback_;       该频道的读回调函数封装器
std::function<void()> write_callback_;      该频道的写回调函数封装器

成员方法:

void HandleEvent();                                             根据该频道发生的事件类型(ready_events_)与所监听的事件类型比较进行处理
void EnableRead();                                              为文件描述符fd_在_loop的epoll实例化对象中注册关于读取事件和紧急数据可读事件的监听
int GetFd();                                                    获取该频道的文件描述符fd_
uint32_t GetListenEvents();                                     获取文件描述符所监听的特定事件类型
uint32_t GetReadyEvents();                                      获取该频道（或者说该频道监听的文件描述符）就绪的事件类型
bool GetInEpoll();                                              获取该文件描述符是否在epoll的监听中
void SetInEpoll(bool in = true);                                设置该文件描述符的in_epoll_为true
void UseET();                                                   使用epoll的ET模式
void SetReadyEvents(uint32_t ev);                               设置该频道就绪的事件类型
void SetReadCallback(std::function<void()> const &callback);    设置该频道所监听的特定事件类型就绪时调用的回调函数


构造函数:

Channel(EventLoop* loop, int fd)             传入获取事件的事件循环loop和所需要监听事件的文件描述符fd