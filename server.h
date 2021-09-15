#ifndef P2PBOTNET_SERVER_H
#define P2PBOTNET_SERVER_H

int init_server(const int);

// 服务端处理命令线程
void* threadedServer(void *);
#endif