介绍一下类InetAddress:

成员属性：

struct sockaddr_in addr_        套接字地址结构的实例化


成员方法:

void SetAddr(sockaddr_in addr);                             设置套接字地址结构
sockaddr_in GetAddr();                                      获取套接字地址结构
const char* GetIp();                                        获取该套接字地址结构的ip
uint16_t GetPort();                                         获取该套接字地址结构的端口


构造函数:

InetAddress();                                              默认构造:对属性addr进行初始化
InetAddress(const char* ip, uint16_t port);                 用所传ip、port对套接字地址结构的属性进行赋值（协议族默认为TCP/IP）