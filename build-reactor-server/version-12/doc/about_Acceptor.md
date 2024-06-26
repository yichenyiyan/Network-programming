介绍一下类Acceptor:

成员属性:

EventLoop* loop_;                                       接收组件所服务的事件循环，处理来自该事件循环的新连接
Socket* sock_;                                          接收组件所监听的Socket实例化对象
Channel* channel_;                                      接收组件所处频道
std::function<void(Socket*)> new_connection_callback_;  接收组件处理连接的回调


成员方法:

void AcceptConnection();                                                        处理新连接的方法
void SetNewConnectionCallback(std::function<void(Socket*)> const &callback);    设置接收组件处理连接的回调


构造函数:

Acceptor(EventLoop* loop)          传入该接收组件所需服务的事件循环，并创建监听套接字进行绑定、监听，并为该套接字创建频道赋值给成员属性channel_