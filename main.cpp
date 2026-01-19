#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>


int main(int argc, char const *argv[])
{
    const char* multAddr = "239.255.255.250";
    const int multPort = 4001;
    const int multPortRcv = 4002;

    int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    int sock2 = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock1 < 0){
        perror("socket1");
        return -1;
    }

    if(sock2 < 0){
        perror("socket2");
        return -1;
    }

    int ttl = 2;

    if(setsockopt(sock1, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0){
        perror("IP_MULTICAST_TTL 1");
        return -1;
    }

    sockaddr_in dest{};
    sockaddr_in serv{};

    dest.sin_family = AF_INET;
    serv.sin_family = AF_INET;
    dest.sin_port = htons(multPort);
    serv.sin_port = htons(multPortRcv);
    dest.sin_addr.s_addr = inet_addr(multAddr);
    serv.sin_addr.s_addr = INADDR_ANY;


    const char* scan = "{\"msg\": { \"cmd\":\"scan\",\"data\":{\"account_topic\":\"reserve\"}}}";

    ssize_t sent = sendto(sock1, scan, strlen(scan), 0, (sockaddr*)&dest, sizeof(dest));

    if(sent < 0){
        perror("sendto");
        return -1;
    }

    if(bind(sock2, (const struct sockaddr*)&serv, sizeof(serv)) < 0){
        perror("bind failed");
        return -1;
    }

    sockaddr_in cli{};
    socklen_t len;
    int n;
    len  = sizeof(cli);
    char buff[1024];
    n = recvfrom(sock2, buff, 1024, MSG_WAITALL, (struct sockaddr*)&cli, &len);
    buff[n] = '\0';
    std::cout << buff;


    return 0;
}
