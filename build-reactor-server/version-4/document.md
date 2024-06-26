简单的epoll-echo服务器和定制的客户端(客户端依旧是version1)

version 4

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
Socket(int);                     传参构造：将所传实参的值对成员属性进行赋值，所传形参为-1时默认退出程序(主要用于保存客户端的套接字)

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
std::vector<Channel*> poll(int timeout = -1)    默认不设置超时，timeout为超时时长，返回事件发生的文件描述符所在频道的数组
void updateChannel(Channel* ch)                 把成员属性epfd更新到ch所指的新频道上

构造函数:
Epoll()    默认构造:创建epoll实例化并对属性epfd进行赋值，创建失败则退出程序，在堆区开辟一个event_epoll数组（默认容量为1024）

-------------------------------------------------------------------------------------------------

类Channel:

成员属性:
EventLoop* loop;    指向该频道管理的EventLoop类实例化的指针
int fd              该频道的文件描述符
uint32_t events     该频道的事件类型(或者称为这个频道的事件类型)
uint32_t revents    写事件
bool inEpoll        是否在epoll中
std::function<void()> callback 该频道的回调函数（或者称为处理事件的函数）


成员方法:

void enableReading()            设置监视该频道的文件描述符上的读事件
int m_getFd()                   获取该频道上的文件描述符
uint32_t m_getEvents()          获取该频道监听文件描述符的事件
uint32_t m_getRevents()         获取该频道监听文件描述符的事件
bool m_getInEpoll()             该频道监听的文件描述符是否在epoll
void m_setInEpoll()             设置该频道监听的文件描述符其在epoll中
void m_setRevents(uint32_t _ev) 设置成员属性revents值为_ev
 
void handleEvent()              调用该频道绑定的回调函数
void setCallback(std::function<void()> _cb) 设置该频道的函数为_cb

构造函数:
Channel(yichen::EventLoop* _loop, int _fd)    对其属性进行初始化

-------------------------------------------------------------------------------------------------

类EventLoop:

成员属性:
Epoll *ep                指向事件循环所管理的实例化epoll对象的指针
bool quit                事件循环是否退出的标志

成员方法:
void loop()                        我们事件驱动的“主角”:事件循环（源头是epoll的循环检测）
void updateChannel(Channel* ch)    更新事件循环所在的频道，至于其频道监听的套接字通常是监听套接字（即sockfd）

构造函数:
EventLoop()                   默认构造函数，初始化成员属性

-------------------------------------------------------------------------------------------------

类Server:

成员属性:
EventLoop* loop         事件循环


成员方法:
void handleReadEvent(int fd)           处理读事件的函数，fd为所处理（client）的套接字
void newConnection(Socket* serv_sock)  处理新的client连接请求, serv_sock为属性fd为监听套接字的Socket实例化对象、

构造函数:
Server(yichen::EventLoop *_loop)      传入一个指向事件循环类实例化对象的指针对其loop属性进行初始化，
                                    并为其(loop)所在频道创建(监听)套接字

-------------------------------------------------------------------------------------------------
