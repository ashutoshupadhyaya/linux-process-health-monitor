#include "device/device_interface_manager.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

DeviceInterfaceManager::DeviceInterfaceManager()
    : deviceFd(-1)
{
}

DeviceInterfaceManager::~DeviceInterfaceManager()
{
    closeDevice();
}

bool DeviceInterfaceManager::openDevice()
{
    if (deviceFd >= 0)
    {
        return true;
    }

    deviceFd = open("/dev/phmctl", O_RDWR);

    if (deviceFd < 0)
    {
        std::cerr
            << "[DEVICE] Failed to open /dev/phmctl: "
            << std::strerror(errno)
            << " (errno "
            << errno
            << ")."
            << std::endl;

        return false;
    }

    std::cout
        << "[DEVICE] /dev/phmctl opened successfully."
        << std::endl;

    return true;
}

bool DeviceInterfaceManager::writeMessage(
    const std::string& message)
{
    if (deviceFd < 0)
    {
        return false;
    }

    ssize_t written =
        write(deviceFd,
              message.c_str(),
              message.size());

    if (written < 0)
    {
        std::cerr
            << "[DEVICE] Write failed: "
            << std::strerror(errno)
            << std::endl;

        return false;
    }

    if (static_cast<std::size_t>(written) !=
        message.size())
    {
        std::cerr
            << "[DEVICE] Incomplete write."
            << std::endl;

        return false;
    }

    return true;
}

bool DeviceInterfaceManager::readMessage(
    std::string& message)
{
    if (deviceFd < 0)
    {
        return false;
    }

    char buffer[256] = {};

    ssize_t bytesRead =
        read(deviceFd,
             buffer,
             sizeof(buffer) - 1);

    if (bytesRead < 0)
    {
        std::cerr
            << "[DEVICE] Read failed: "
            << std::strerror(errno)
            << std::endl;

        return false;
    }

    buffer[bytesRead] = '\0';
    message = buffer;

    return true;
}

void DeviceInterfaceManager::closeDevice()
{
    if (deviceFd >= 0)
    {
        close(deviceFd);
        deviceFd = -1;

        std::cout
            << "[DEVICE] /dev/phmctl closed."
            << std::endl;
    }
}
