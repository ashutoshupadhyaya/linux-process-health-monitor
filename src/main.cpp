#include "process_monitor.hpp"
#include <iostream>
#include "system_monitor.hpp"
#include "health_analyzer.hpp"

int main()
{
    SystemMonitor monitor;
    ProcessMonitor processMonitor;

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

    auto processes = processMonitor.getProcesses();
    HealthAnalyzer analyzer;
    std::cout << "\nRunning Processes:\n";

    int count = 0;

    for (const auto& process : processes)
{
    std::cout << "PID: " << process.pid
              << " | PPID: " << process.ppid
              << " | Name: " << process.name
              << " | State: " << process.state
              << " | Memory: " << process.memoryKb << " kB"
              << " | CPU: " << process.cpuUsage << "%"
              << std::endl;
        count++;
HealthResult result = analyzer.analyzeProcess(process);

std::cout << "   Health: ";

if (result.status == HealthStatus::NORMAL)
{
    std::cout << "NORMAL";
}
else if (result.status == HealthStatus::WARNING)
{
    std::cout << "WARNING";
}
else
{
    std::cout << "CRITICAL";
}

std::cout << " - " << result.reason << std::endl;
        if (count >= 10)
        {
            break;
        }
    }

    return 0;
}
