简单的epoll-echo服务器和定制的客户端

version 1

所有的实现都在命名空间yichen中实现!

util.hh:
宏DEBUG(expr, errmsg): expr是个判断式子，errmsg为C字符串字面量，当判断为真时打印输出并输出errmsg，并退出程序。

