#ifndef PROCESS_MONITOR_HPP
#define PROCESS_MONITOR_HPP

#include <string>
#include <vector>

struct ProcessInfo
{
    int pid;
    int ppid;
    std::string name;
    std::string state;
    long memoryKb;
    double cpuUsage;
};

class ProcessMonitor
{
public:
    std::vector<ProcessInfo> getProcesses();
};

#endif

