#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

class SystemMonitor
{
public:
    double getCpuUsage();
    double getMemoryUsage();
    double getDiskUsage();
};

#endif
