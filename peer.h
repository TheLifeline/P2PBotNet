//
// Created by user on 2020/8/4.
//

#ifndef P2PBOTNET_PEER_H
#define P2PBOTNET_PEER_H


#include <stdbool.h>
#include <stdint.h>
#include "global.h"

#define BUFFER_SIZE_NETWORK 512
#define MAX_CLIENTS 10

typedef struct Message {
    int state;//通讯状态，200为正常
    int action;//指令等级，1-10为系统指令，>10为特权指令
    char *command;//指令内容；
} Message;
typedef struct
{
    uint32_t ServantIP;
    uint16_t ServantPort;
    char *BotID;
    uint32_t FailCount;
} Peer;
Peer peer_list[10];//维护一个peer_list
char *encodeMessage(Message);

Message decodeMessage(char *);

void sendMessage(int, char *);

char *receiveMessage(int, char *);


#endif //P2PBOTNET_PEER_H
