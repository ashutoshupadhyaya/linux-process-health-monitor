#include "process_monitor.hpp"

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <thread>
#include <chrono>

long long getTotalCpuTicks()
{
    std::ifstream file("/proc/stat");

    if (!file.is_open())
    {
        return 0;
    }

    std::string line;
    std::getline(file, line);

    std::istringstream stream(line);
    std::string cpu;

    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;

    stream >> cpu
           >> user
           >> nice
           >> system
           >> idle
           >> iowait
           >> irq
           >> softirq;

    return user + nice + system + idle +
           iowait + irq + softirq;
}

long long getProcessCpuTicks(int pid)
{
    std::ifstream file("/proc/" + std::to_string(pid) + "/stat");

    if (!file.is_open())
    {
        return 0;
    }

    std::string line;
    std::getline(file, line);

    std::size_t closingBracket = line.rfind(')');

    if (closingBracket == std::string::npos)
    {
        return 0;
    }

    std::string remaining = line.substr(closingBracket + 2);

    std::istringstream stream(remaining);

    std::string state;
    long long value;

    stream >> state;

    for (int i = 0; i < 10; i++)
    {
        stream >> value;
    }

    long long utime = 0;
    long long stime = 0;

    stream >> utime >> stime;

    return utime + stime;
}

std::vector<ProcessInfo> ProcessMonitor::getProcesses()
{
    std::vector<ProcessInfo> processes;

    long long totalCpuBefore = getTotalCpuTicks();

    std::unordered_map<int, long long> processCpuBefore;

    DIR* procDirectory = opendir("/proc");

    if (procDirectory == nullptr)
    {
        return processes;
    }

    struct dirent* entry;

    while ((entry = readdir(procDirectory)) != nullptr)
    {
        std::string directoryName = entry->d_name;

        if (directoryName.empty() ||
            directoryName.find_first_not_of("0123456789") != std::string::npos)
        {
            continue;
        }

        int pid = std::stoi(directoryName);

        processCpuBefore[pid] = getProcessCpuTicks(pid);

        std::string statusPath =
            "/proc/" + directoryName + "/status";

        std::ifstream statusFile(statusPath);

        if (!statusFile.is_open())
        {
            continue;
        }

        ProcessInfo process{};

        process.pid = pid;
        process.memoryKb = 0;
        process.cpuUsage = 0.0;

        std::string line;

        while (std::getline(statusFile, line))
        {
            if (line.rfind("Name:", 0) == 0)
            {
                process.name = line.substr(5);

                if (!process.name.empty() &&
                    process.name[0] == '\t')
                {
                    process.name.erase(0, 1);
                }
            }
            else if (line.rfind("State:", 0) == 0)
            {
                process.state = line.substr(6);

                if (!process.state.empty() &&
                    process.state[0] == '\t')
                {
                    process.state.erase(0, 1);
                }
            }
            else if (line.rfind("PPid:", 0) == 0)
            {
                std::istringstream stream(line.substr(5));
                stream >> process.ppid;
            }
            else if (line.rfind("VmRSS:", 0) == 0)
            {
                std::istringstream stream(line.substr(6));
                stream >> process.memoryKb;
            }
        }

        processes.push_back(process);
    }

    closedir(procDirectory);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    long long totalCpuAfter = getTotalCpuTicks();

    long long totalCpuDifference =
        totalCpuAfter - totalCpuBefore;

    if (totalCpuDifference > 0)
    {
        for (auto& process : processes)
        {
            long long processCpuAfter =
                getProcessCpuTicks(process.pid);

            auto it =
                processCpuBefore.find(process.pid);

            if (it != processCpuBefore.end())
            {
                long long processCpuDifference =
                    processCpuAfter - it->second;

                if (processCpuDifference >= 0)
                {
                    process.cpuUsage =
                        static_cast<double>(
                            processCpuDifference) /
                        totalCpuDifference * 100.0;
                }
            }
        }
    }

    return processes;
}
