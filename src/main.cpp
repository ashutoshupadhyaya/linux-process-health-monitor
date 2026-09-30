#include <iostream>
#include "system_monitor.hpp"

int main()
{
    SystemMonitor monitor;

    std::cout << "Linux Process Health Monitor started." << std::endl;
    std::cout << "CPU Usage: " << monitor.getCpuUsage() << "%" << std::endl;

    return 0;
}
