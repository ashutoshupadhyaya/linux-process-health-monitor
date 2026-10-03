#include "monitoring/system_monitor.hpp"

#include <sys/statvfs.h>

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

namespace
{

bool readCpuCounters(
    unsigned long long& idle,
    unsigned long long& total)
{
    std::ifstream file("/proc/stat");

    if (!file)
    {
        return false;
    }

    std::string line;

    if (!std::getline(file, line))
    {
        return false;
    }

    std::istringstream stream(line);

    std::string cpu;

    unsigned long long user = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idleTime = 0;
    unsigned long long iowait = 0;
    unsigned long long irq = 0;
    unsigned long long softirq = 0;
    unsigned long long steal = 0;

    if (!(stream >> cpu
                 >> user
                 >> nice
                 >> system
                 >> idleTime
                 >> iowait
                 >> irq
                 >> softirq
                 >> steal))
    {
        return false;
    }

    idle = idleTime + iowait;

    total = user +
            nice +
            system +
            idleTime +
            iowait +
            irq +
            softirq +
            steal;

    return true;
}

} // namespace

double SystemMonitor::getCpuUsage()
{
    unsigned long long idleBefore = 0;
    unsigned long long totalBefore = 0;

    if (!readCpuCounters(idleBefore, totalBefore))
    {
        return 0.0;
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    unsigned long long idleAfter = 0;
    unsigned long long totalAfter = 0;

    if (!readCpuCounters(idleAfter, totalAfter))
    {
        return 0.0;
    }

    const unsigned long long totalDelta =
        totalAfter - totalBefore;

    const unsigned long long idleDelta =
        idleAfter - idleBefore;

    if (totalDelta == 0)
    {
        return 0.0;
    }

    double usage =
        (static_cast<double>(totalDelta - idleDelta) /
         static_cast<double>(totalDelta)) *
        100.0;

    if (usage < 0.0)
    {
        usage = 0.0;
    }

    if (usage > 100.0)
    {
        usage = 100.0;
    }

    return usage;
}

double SystemMonitor::getMemoryUsage()
{
    std::ifstream file("/proc/meminfo");

    if (!file)
    {
        return 0.0;
    }

    long long totalMemoryKb = 0;
    long long availableMemoryKb = 0;

    std::string label;
    long long value;
    std::string unit;

    while (file >> label >> value >> unit)
    {
        if (label == "MemTotal:")
        {
            totalMemoryKb = value;
        }
        else if (label == "MemAvailable:")
        {
            availableMemoryKb = value;
        }

        if (totalMemoryKb > 0 &&
            availableMemoryKb > 0)
        {
            break;
        }
    }

    if (totalMemoryKb <= 0)
    {
        return 0.0;
    }

    double usage =
        (static_cast<double>(
             totalMemoryKb - availableMemoryKb) /
         static_cast<double>(totalMemoryKb)) *
        100.0;

    if (usage < 0.0)
    {
        usage = 0.0;
    }

    if (usage > 100.0)
    {
        usage = 100.0;
    }

    return usage;
}

double SystemMonitor::getDiskUsage()
{
    struct statvfs filesystemInfo{};

    if (statvfs("/", &filesystemInfo) != 0)
    {
        return 0.0;
    }

    const unsigned long long totalBlocks =
        filesystemInfo.f_blocks;

    const unsigned long long freeBlocks =
        filesystemInfo.f_bfree;

    if (totalBlocks == 0)
    {
        return 0.0;
    }

    const unsigned long long usedBlocks =
        totalBlocks - freeBlocks;

    double usage =
        (static_cast<double>(usedBlocks) /
         static_cast<double>(totalBlocks)) *
        100.0;

    if (usage < 0.0)
    {
        usage = 0.0;
    }

    if (usage > 100.0)
    {
        usage = 100.0;
    }

    return usage;
}
