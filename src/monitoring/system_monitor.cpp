#include "system_monitor.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>

double SystemMonitor::getCpuUsage()
{
    auto readCpuStats = []()
    {
        std::ifstream file("/proc/stat");
        std::string line;

        long long user = 0;
        long long nice = 0;
        long long system = 0;
        long long idle = 0;
        long long iowait = 0;
        long long irq = 0;
        long long softirq = 0;

        if (!file.is_open())
        {
            return std::vector<long long>{};
        }

        std::getline(file, line);

        std::istringstream stream(line);
        std::string cpu;

        stream >> cpu
               >> user
               >> nice
               >> system
               >> idle
               >> iowait
               >> irq
               >> softirq;

        return std::vector<long long>{
            user, nice, system, idle,
            iowait, irq, softirq
        };
    };

    auto first = readCpuStats();

    if (first.empty())
    {
        return 0.0;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto second = readCpuStats();

    if (second.empty())
    {
        return 0.0;
    }

    long long firstTotal = 0;
    long long secondTotal = 0;

    for (long long value : first)
    {
        firstTotal += value;
    }

    for (long long value : second)
    {
        secondTotal += value;
    }

    long long totalDifference = secondTotal - firstTotal;

    long long firstIdle = first[3] + first[4];
    long long secondIdle = second[3] + second[4];

    long long idleDifference = secondIdle - firstIdle;

    if (totalDifference <= 0)
    {
        return 0.0;
    }

    return static_cast<double>(
        totalDifference - idleDifference
    ) / totalDifference * 100.0;
}
double SystemMonitor::getMemoryUsage()
{
    std::ifstream file("/proc/meminfo");

    if (!file.is_open())
    {
        return 0.0;
    }

    std::string key;
    long long value;
    std::string unit;

    long long memTotal = 0;
    long long memAvailable = 0;

    while (file >> key >> value >> unit)
    {
        if (key == "MemTotal:")
        {
            memTotal = value;
        }
        else if (key == "MemAvailable:")
        {
            memAvailable = value;
        }

        if (memTotal > 0 && memAvailable > 0)
        {
            break;
        }
    }

    if (memTotal == 0)
    {
        return 0.0;
    }

    return static_cast<double>(memTotal - memAvailable)
           / memTotal * 100.0;
}
#include <sys/statvfs.h>

double SystemMonitor::getDiskUsage()
{
    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0)
    {
        return 0.0;
    }

    unsigned long long totalSpace =
        static_cast<unsigned long long>(filesystem.f_blocks) *
        filesystem.f_frsize;

    unsigned long long availableSpace =
        static_cast<unsigned long long>(filesystem.f_bavail) *
        filesystem.f_frsize;

    if (totalSpace == 0)
    {
        return 0.0;
    }

    unsigned long long usedSpace = totalSpace - availableSpace;

    return static_cast<double>(usedSpace) /
           totalSpace * 100.0;
}
