介绍一下类Buffer:

成员属性:
std::string buf                                    缓存区底层实现数据结构

成员方法:

void Append(const char *_str, int _size);          相当于string的push_back,_str为C字面量指针，_size为_str大小
ssize_t Size();                                    返回缓存区大小
const char* ToStr();                               返回C风格的字符串字面量
void Clear();                                      清除缓冲区
void Getline();                                    清空缓冲区原有内容，从标准输入读取字符到缓冲区
void SetBuf(const char* buf);                      清空缓冲区原有内容，将内容设定为buf

构造函数:
Buffer()                                            default