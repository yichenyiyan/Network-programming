简单的epoll-echo服务器和定制的客户端(客户端依旧是version1)

version 9

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


成员方法:
void setInetAddr(sockaddr_in _addr, socklen_t _addr_len)    设置该地址的信息
sockaddr_in getAddr()                                       获取地址信息结构体
socklen_t getAddr_len()                                     获取地址信息结构体的长度

构造函数:
InetAddress()                                 默认构造:对属性addr进行初始化
InetAddress(const char* ip, uint16_t port)    用所传ip、port对addr的属性进行赋值（协议族默认为TCP/IP）

-------------------------------------------------------------------------------------------------

类Epoll:

成员属性:
int epfd                        epoll实例化的文件描述符
struct epoll_event *events      epoll_event结构体数组

成员方法:
std::vector<Channel*> poll(int timeout = -1)    默认不设置超时，timeout为超时时长，返回事件发生的文件描述符所在频道的数组
void updateChannel(Channel* ch)                 把成员属性epfd更新到ch所指的新频道上

构造函数:
Epoll()    默认构造:创建epoll实例化并对属性epfd进行赋值，创建失败则退出程序，在堆区开辟一个event_epoll数组（默认容量为1024）

-------------------------------------------------------------------------------------------------

类Channel:

成员属性:
EventLoop* loop;                      指向该频道管理的EventLoop类实例化的指针
int fd                                该频道的文件描述符
uint32_t events                       该频道的事件类型(或者称为这个频道的事件类型)
uint32_t ready                        事件类型
bool inEpoll                          是否在epoll中
std::function<void()> callback        该频道的回调函数（或者称为处理事件的函数）
bool usePthreadPool;                  是否使用线程池
std::function<void()> readCallback;   该频道的读回调函数
std::function<void()> writeCallback;  该频道的写回调函数

成员方法:

void useET()                                使用epoll的ET模式
void handleEvent()                          根据该频道的事件类型(events)进行处理
void enableRead()                           设置监视该频道的文件描述符上的读事件
int m_getFd()                               获取该频道上的文件描述符
uint32_t m_getEvents()                      获取该频道监听文件描述符的事件
uint32_t m_getReady()                       获取该频道监听文件描述符的事件
bool m_getInEpoll()                         获取该频道监听的文件描述符是否在epoll
void m_setInEpoll(bool _in = true)          设置该频道监听的文件描述符其在epoll中
void m_setReady(uint32_t)                   设置该频道文件描述符监听的事件
void setReadCallback(std::function<void()>) 设置读回调       
void setUseThreadPool(bool use = true)      设置写回调

构造函数:
Channel(yichen::EventLoop* _loop, int _fd)    对其属性进行初始化

-------------------------------------------------------------------------------------------------

类ThreadPool:

成员属性:
bool stop;                                       线程池是否停止运行
std::mutex tasks_mtx;                            池内互斥锁
std::condition_variable cv;                      池内条件变量
std::vector<std::thread> threads;                线程数组
std::queue<std::function<void()>> tasks;         任务队列

成员方法:
void addTask(std::function<void()> func)         往线程池中添加任务

构造函数:
ThreadPool(int size)                             创建有size个线程的线程池

-------------------------------------------------------------------------------------------------


类EventLoop:

成员属性:
Epoll *ep                指向事件循环所管理的实例化epoll对象的指针
bool quit                事件循环是否退出的标志
ThreadPool* pthreadPool  工作线程池

成员方法:
void loop()                                     我们事件驱动的“主角”:事件循环（源头是不断调用epoll_wait进行循环检测）
void updateChannel(Channel* ch)                 更新事件循环所在的频道，至于其频道监听的套接字通常是监听套接字（即sockfd）
void addThread(std::function<void()> task)      往工作线程池中添加任务

构造函数:
EventLoop()                   默认构造函数，初始化成员属性

-------------------------------------------------------------------------------------------------

类Acceptor:

成员属性:
EventLoop* loop                                       新连接的事件循环
Socket* sock                                          监听的套接字所在的Socket实例化对象
Channel* acceptChannel;                               accepter所在频道
std::function<void(Socket*)> newConnectionCallback    处理新连接的回调函数

成员方法:
void acceptConnection()                                           执行处理新连接的回调
void setNewConnectionCallback(std::function<void(Socket*)> _cb)   设置处理新连接的回调函数

构造函数:
Acceptor(EventLoop *_loop)    传入一个指向事件循环类实例化对象的指针对其loop属性进行初始化，
                                    并为其(loop)所在频道创建(监听)套接字

-------------------------------------------------------------------------------------------------

类Connection:

成员属性:
EventLoop* loop                                        连接所处的事件循环
Socket* sock                                           类Connection的套接字
Channel* channel                                       连接所属频道
std::function<void(Socket*)> deleteConnectionCallback  删除连接的回调

成员方法:
void echo(int sockfd)                                               处理客户端的echo方法
void setDeleteConnectionCallback(std::function<void(Socket*)> _cb)  设置该连接的删除连接回调

构造函数:
Connection(EventLoop *_loop, Socket *_sock)        传入该连接的Socket,及其Socket所属的事件循环

-------------------------------------------------------------------------------------------------

类Buffer:

成员属性:
std::string buf            缓存区底层实现数据结构

成员方法:
void append(const char* _str, int _size)            相当于string的push_back, _str为C字面量指针，_size为_str大小
ssize_t size()                                      返回缓存区大小
const char* c_str()                                 返回C字符串字面量
void clear()                                        清除缓冲区


构造函数:
Buffer()                                            null

-------------------------------------------------------------------------------------------------


类Server:

成员属性:
EventLoop* loop                                     事件循环
Acceptor *acceptor                                  连接处理者
std::unordered_map<int, Connection*> connections    服务器管理的套接字及其连接的映射


成员方法:
void newConnection(Socket* sock)       处理新的client连接请求, sock为属性fd为监听套接字的Socket实例化对象
void deleteConnection(Socket* sock);   删除某个连接的回调

构造函数:
Server(yichen::EventLoop *_loop)      传入一个指向事件循环类实例化对象的指针对其loop属性进行初始化，
                                    并为其(loop)创建一个accepter,并为accepter设置其回调函数

-------------------------------------------------------------------------------------------------


