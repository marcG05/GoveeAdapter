#include "GooveConnector.h"
#include <iostream>

nlohmann::json GooveConnector::recv(char* ip_out)
{
    if(sockRecv == 0){
        sockRecv = socket(AF_INET, SOCK_DGRAM, 0);

        if(sockRecv < 0){
            perror("sockRecv");
            return {};

        }
        
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(MULTI_PORT_RECV);
        recvAddr.sin_addr.s_addr = INADDR_ANY;

        if(bind(sockRecv, (const struct sockaddr*)&recvAddr, sizeof(recvAddr)) < 0){
            perror("bind failed");
            return {};
        }
    }

    sockaddr_in cli{};
    socklen_t len;
    int n;
    len  = sizeof(cli);
    char buff[4096];
    n = recvfrom(sockRecv, buff, 4096, MSG_WAITALL, (struct sockaddr*)&cli, &len);
    buff[n] = '\0';

    if(n < 0){
        perror("recv");
        return {};
    }

    if(inet_ntop(AF_INET, &cli.sin_addr, ip_out, INET_ADDRSTRLEN) == nullptr){
        perror("ipAddr");
    }
    return nlohmann::json::parse(buff);
}

/**
 * nlohmann::json GooveConnector::recv(char ip_out[INET_ADDRSTRLEN])
{
    if (sockRecv == 0) {
        sockRecv = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockRecv < 0) {
            perror("socket");
            return {};
        }

        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(MULTI_PORT_RECV);
        recvAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockRecv, (struct sockaddr*)&recvAddr, sizeof(recvAddr)) < 0) {
            perror("bind");
            return {};
        }
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sockRecv, &fds);

    timeval tv{};
    tv.tv_sec = 0;      // no blocking
    tv.tv_usec = 0;

    int ready = select(sockRecv + 1, &fds, nullptr, nullptr, &tv);
    if (ready <= 0) {
        return {};  // no data (interrupt-style return)
    }

    sockaddr_in cli{};
    socklen_t len = sizeof(cli);
    char buff[1024];

    int n = recvfrom(sockRecv, buff, sizeof(buff) - 1, 0,
                     (struct sockaddr*)&cli, &len);
    if (n <= 0) return {};

    buff[n] = '\0';
    inet_ntop(AF_INET, &cli.sin_addr, ip_out, INET_ADDRSTRLEN);

    return nlohmann::json::parse(buff);
}

 */

GooveConnector::GooveConnector()
{
}

void GooveConnector::discover()
{
    if(sockDisc == 0){
        sockDisc = socket(AF_INET, SOCK_DGRAM, 0);

        if(sockDisc < 0){
            perror("sockDisc");
            return;

        }
        
        discAddr.sin_family = AF_INET;
        discAddr.sin_port = htons(MULTI_PORT_DISC);
        discAddr.sin_addr.s_addr = inet_addr(MULTI_ADDR);
    }



    nlohmann::json j = R"({
        "msg" : {
            "cmd" : "scan",
            "data" : {
                "account_topic" : "reserve"
            }
        }
    })"_json;

    ssize_t sent = sendto(sockDisc, j.dump().c_str(),  j.dump().size(), 0, (sockaddr*)&discAddr, sizeof(discAddr));
    char ipd[INET_ADDRSTRLEN];
    nlohmann::json scanned = recv(ipd);
    devices.push_back(GooveDevice(scanned["msg"]["data"]["sku"], ipd));
}

GooveDevice* GooveConnector::getDevice(std::string ip)
{
    for(GooveDevice& d : devices){
        if(d.getIP() == ip) return &d;
    }
    return nullptr;
}

GooveDevice* GooveConnector::getIndex(unsigned int i)
{
    if(i >= devices.size()) return nullptr;
    return &devices[i];
}

size_t GooveConnector::getLen()
{
    return devices.size();
}

nlohmann::json GooveConnector::sendCommand(std::string ip, const nlohmann::json &req, bool ret)
{
    GooveDevice* dev = getDevice(ip);
    if(dev == nullptr) return R"({
        "error" : "NO DEV"
    })"_json;

    if(sockSend == 0){
        sockSend = socket(AF_INET, SOCK_DGRAM, 0);

        if(sockSend < 0){
            perror("sockSend");
            return {};
        }
    }

    sockaddr_in cli{};
    cli.sin_family = AF_INET;
    cli.sin_port=  htons(MULTI_PORT_SEND);
    cli.sin_addr.s_addr = inet_addr(ip.c_str());

    ssize_t stn = sendto(sockSend, req.dump().c_str(), req.dump().size(), 0, (sockaddr*)&cli, sizeof(cli));

    if(stn < 0){
        perror("send");
        return {};
    }

    if(ret){
        char ipo[INET_ADDRSTRLEN];
        nlohmann::json js = recv(ipo);
        return js;
    }

    return {{
        "success", "command sent"
    }};
    
}

GooveConnector::~GooveConnector()
{
    if(sockDisc > 0) close(sockDisc);
    if(sockRecv > 0) close(sockRecv);
    if(sockSend > 0) close(sockSend);
}
