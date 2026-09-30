#include <iostream>
#include "system_monitor.hpp"

int main()
{
    SystemMonitor monitor;

    std::cout << "Linux Process Health Monitor started." << std::endl;

    std::cout << "CPU Usage: "
              << monitor.getCpuUsage()
              << "%" << std::endl;

    std::cout << "Memory Usage: "
              << monitor.getMemoryUsage()
              << "%" << std::endl;

    std::cout << "Disk Usage: "
              << monitor.getDiskUsage()
              << "%" << std::endl;

    return 0;
}
