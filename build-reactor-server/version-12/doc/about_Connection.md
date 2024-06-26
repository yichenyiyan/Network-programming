介绍一下类Connection:


成员属性:

enum class State : const int {
    Invalid = 1, //无效
    Handshaking, //三次握手ing
    Connected,   //已连接
    Closed,      //已断开连接(关闭)
    Failed       //失败
};                                                              连接状态码

EventLoop* loop_;                                               
Socket* sock_;
Channel* channel_;
State state_{State::Invalid};
Buffer* read_buffer_;
Buffer* send_buffer_;
std::function<void(Socket*)> delete_connectioin_callback_;
std::function<void(Connection*)> on_connect_callback_;



成员方法:

void ReadNonBlocking();                                                             ‘Read’函数，非阻塞读
void WriteNonBlocking();                                                            ‘Write’函数，非阻塞写
void ReadBlocking();                                                                ‘Read’函数，阻塞读(客户端套接字调用)
void WriteBlocking();                                                               ‘Write’函数，阻塞写(客户端套接字调用)
void Read();                                                                        根据套接字是否为非阻塞式调用‘Read’函数
void Write();                                                                       根据套接字是否为非阻塞式调用‘Write’函数
void SetDeleteConnectionCallback(std::function<void(Socket*)> const &callback);     设置删除连接的回调
void SetOnConnectCallback(std::function<void(Connection*)> const &callback);        设置连接中监听的特定事件就绪时调用的回调
State GetState();                                                                   获取服务器当前与该客户的连接状态码
void Close();                                                                       关闭该连接
void SetSendBuffer(const char* str);                                                设置与该连接的发送缓冲区的内容为str
Buffer* GetReadBuffer();                                                            返回成员属性read_buffer_
const char* ReadBuffer();                                                           返回read_buffer_调用To_Str()
Buffer* GetSendBuffer();                                                            返回成员属性send_buffer_
const char* SendBuffer();                                                           返回send_buffer_调用To_Str()
void GetlineSendBuffer();                                                           send_buffer_调用Getline()
Socket* GetSocket();                                                                获取该连接的Socket类实例化对象
void OnConnect(std::function<void()> fn);


构造函数:

Connection(EventLoop* loop, Socket* sock)           传入该连接所属的事件循环、该该连接的套接字所在实例化对象，并为该连接创建频道赋值给属性channel_