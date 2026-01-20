
#include <iostream>
#include "GooveConnector.h"
#include "GooveCommands.h"


int main(int argc, char const *argv[])
{
    GooveConnector c;

    std::cout <<"disco\n";

    c.discover();


    std::cout << "Goove discovered : " << (int)c.getLen() << std::endl;

    std::cout << c.getIndex(0)->getIP() << std::endl;
    nlohmann::json stat = c.sendCommand("192.168.2.132", devStatus, true);

    std::cout << stat.dump() <<std::endl;


    nlohmann::json colr = c.sendCommand("192.168.2.132", color(std::stoi(argv[1]),std::stoi(argv[2]),std::stoi(argv[3])));
    std::cout << colr.dump() << std::endl;
    return 0;
}
