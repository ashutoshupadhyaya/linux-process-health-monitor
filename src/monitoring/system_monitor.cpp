#include "system_monitor.hpp"
#include <fstream>
#include <string>
#include <sstream>

double SystemMonitor::getCpuUsage()
{
    std::ifstream file("/proc/stat");
    std::string line;

    if (!file.is_open())
    {
        return 0.0;
    }

    std::getline(file, line);

    std::istringstream stream(line);

    std::string cpu;
    long long user;
    long long nice;
    long long system;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;

    stream >> cpu
           >> user
           >> nice
           >> system
           >> idle
           >> iowait
           >> irq
           >> softirq;    
long long total = user + nice + system + idle + iowait + irq + softirq;
long long idleTime = idle + iowait;

if (total == 0)
{
    return 0.0;
}

return (static_cast<double>(total - idleTime) / total) * 100.0;
}
