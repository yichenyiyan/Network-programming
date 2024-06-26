介绍一下项目中用到的工具宏：

DEBUG(expr, errmsg)   expr为关系判断式,errmsg为错误提示码，如果expr判断为true, 则打印错误码并退出程序(这里没有选择断言，而是exit()来退出程序)

DISALLOW_COPY(cname)  禁止拷贝构造及拷贝赋值，cname为类名

DISALLOW_MOVE(cname)  禁止移动构造及移动赋值，cname为类名

DISALLOW_COPY_AND_MOVE(cname) 拷贝和移动构造都禁止，cname为类名

ASSERT(expr, message) expr为关系判断式,errmsg为错误提示码，如果expr判断为true, 则打印错误码并断言

UNREACHABLE(message)  抛出异常(打印message)
