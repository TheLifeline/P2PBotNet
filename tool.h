//
// Created by jiemin on 2020/8/3.
//

#ifndef P2PBOTNET_TOOL_H
#define P2PBOTNET_TOOL_H
#pragma once
#include <stdint.h>
#include <stdio.h>
typedef struct
{
    uint32_t IP;
    uint16_t Port;
} Input;

int checkipstr(char const *ipstr, char ip[4][4]);
Input *check_input(int argc, char const *argv[]);
#endif //P2PBOTNET_TOOL_H
