介绍一下类Server:


成员属性:

EventLoop* main_reactor_;                               主Reactor
Acceptor* acceptor_;                                    接收组件
std::map<int, Connection*> connections_;                套接字与类Connection的映射表
std::vector<EventLoop*> sub_reactors_;                  Sub_Reators
ThreadPool* thread_pool_;                               线程池
std::function<void(Connection*)> on_connect_callback_;  与客户端连接之间的处理程序(回调或者称为所谓的业务)


成员方法:

void NewConnection(Socket* sock);                       处理新连接的流程函数
void DeleteConnection(Socket* sock);                    删除已有连接的流程函数
void OnConnect(std::function<void(Connection*)> fn);    业务流程


构造函数:

Server(EventLoop* loop)           传入一个事件循环、创建一个acceptor为其绑定处理新连接的函数为Server成员方法NewConnection，
                            并根据硬件条件创建一个有适宜数量(size)线程的线程池、size个sub_reactor并将其EventLoop::loop添加到线程池任务中