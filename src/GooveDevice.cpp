#include "GooveDevice.h"

GooveDevice::GooveDevice(string s, string i)
{
    sku = s;
    ip = i;
}

const string GooveDevice::getSKU()
{
    return sku;
}

const string GooveDevice::getIP()
{
    return ip;
}

GooveDevice::~GooveDevice()
{
}
