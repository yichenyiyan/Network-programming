介绍一下类Socket:


成员属性:

int fd_;                            套接字


成员方法:

void Bind(InetAddress* addr);                           根据传入的套接字地址结构对fd_进行bind()
void Listen();                                          对bind()的套接字地址结构的端口进行监听
int Accept(InetAddress* addr);                          根据传入的(一般是客户端的)套接字地址信息进行accept() (此方法一般是服务器使用)
void Connect(InetAddress *addr);                        根据传入的(一般是服务器的)套接字地址结构信息进行connect (此方法一般是客户端使用)
void Connect(const char* ip, uint16_t port);            传入服务器的ip和端口与服务器进行连接  (此方法一般是客户端使用)
void SetNonBlocking();                                  设置fd_为非阻塞
bool IsNonBlocking();                                   fd_是否为非阻塞
int GetFd();                                            获取fd_


构造函数:

Socket();                                               默认跟你创建一个套接字并用socket()返回值对成员属性fd_赋值
Socket(int fd);                                         根据传入的套接字对fd_赋值