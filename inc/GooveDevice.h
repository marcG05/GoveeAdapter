#pragma once
#include <string>

using namespace std;


class GooveDevice
{
private:
    string sku;
    string ip;
public:
    GooveDevice(string s, string i);

    const string getSKU();
    const string getIP();
    ~GooveDevice();
};

