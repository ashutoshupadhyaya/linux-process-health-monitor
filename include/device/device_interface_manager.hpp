#ifndef DEVICE_INTERFACE_MANAGER_HPP
#define DEVICE_INTERFACE_MANAGER_HPP

#include <string>

class DeviceInterfaceManager
{
public:
    DeviceInterfaceManager();
    ~DeviceInterfaceManager();

    bool openDevice();
    bool writeMessage(const std::string& message);
    bool readMessage(std::string& message);
    void closeDevice();

private:
    int deviceFd;
};

#endif
