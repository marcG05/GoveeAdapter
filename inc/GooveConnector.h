#pragma once

#include <nlohmann/json.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include "GooveDevice.h"
#include <vector>

#define MULTI_PORT_DISC 4001
#define MULTI_PORT_RECV 4002
#define MULTI_PORT_SEND 4003
#define MULTI_ADDR "239.255.255.250"


class GooveConnector
{
private:
    std::vector<GooveDevice> devices;
    nlohmann::json recv(char* ip_out);
    int sockDisc = 0, sockRecv = 0, sockSend = 0;
    sockaddr_in recvAddr, discAddr;
public:
    GooveConnector();

    void discover();

    GooveDevice* getDevice(std::string ip);

    GooveDevice* getIndex(unsigned int i);

    size_t getLen();

    nlohmann::json sendCommand(std::string ip, const nlohmann::json& req, bool ret= false);
    ~GooveConnector();
};
