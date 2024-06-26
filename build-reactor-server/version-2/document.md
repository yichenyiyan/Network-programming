简单的epoll-echo服务器和定制的客户端

version 2

所有的实现都在命名空间yichen中实现!

宏DEBUG(expr, errmsg): expr是个判断式子，errmsg为C字符串字面量，当判断为真时打印输出并输出errmsg，并退出程序, 所在头文件util.hh。

-------------------------------------------------------------------------------------------------

类Socket:

成员属性：
int fd   套接字（文件描述符）

成员方法：
void m_bind(InetAddress* addr)   对bind()函数的封装
void m_listen()                  对listen()函数的封装
void m_accept(InetAddress* addr) 对accept()函数的封装
void setnonblocking()            将成员属性 fd 设置为非阻塞
int m_getFd()                    返回套接字的值

构造函数:
Socket();                        默认构造：调用socket()函数对成员属性fd进行赋值，失败默认推出程序
Socket(int);                     传参构造：将所传实参的值对成员属性进行赋值，所传形参为-1时默认退出程序

-------------------------------------------------------------------------------------------------

类InetAddress:

成员属性：
struct sockaddr_in addr         结构体sockaddr_in的实例化
socklen_t addr_len              结构体sockaddr_in的大小


构造函数:
InetAddress()                                 默认构造:对属性addr进行初始化
InetAddress(const char* ip, uint16_t port)    用所传ip、port对addr的属性进行赋值（协议族默认为TCP/IP）

-------------------------------------------------------------------------------------------------

类Epoll:

成员属性:
int epfd                        epoll实例化的文件描述符
struct epoll_event *events      epoll事件结构体指针

成员方法:
void addFd(int fd, uint32_t op)                 fd为需要添加到epoll红黑树上监听的文件描述符, op为所需要监听的事件类型
std::vector<epoll_event> poll(int timeout = -1) 默认不设置超时，timeout为超时时长，返回事件发生的文件描述符数组

构造函数:
Epoll()    默认构造:创建epoll实例化并对属性epfd进行赋值，创建失败则退出程序，在堆区开辟一个event_epoll数组

-------------------------------------------------------------------------------------------------
