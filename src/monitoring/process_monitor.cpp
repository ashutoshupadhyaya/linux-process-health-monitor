#include "monitoring/process_monitor.hpp"

#include <dirent.h>
#include <unistd.h>

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

namespace
{

struct ProcessSnapshot
{
    int ppid = 0;
    char state = '?';
    long memoryKb = 0;
    unsigned long long cpuJiffies = 0;
};

bool isNumeric(const std::string& value)
{
    if (value.empty())
    {
        return false;
    }

    for (char ch : value)
    {
        if (ch < '0' || ch > '9')
        {
            return false;
        }
    }

    return true;
}

bool readProcessSnapshot(
    const std::string& pidString,
    ProcessSnapshot& snapshot)
{
    const std::string statPath =
        "/proc/" + pidString + "/stat";

    std::ifstream statFile(statPath);

    if (!statFile)
    {
        return false;
    }

    std::string statLine;

    if (!std::getline(statFile, statLine))
    {
        return false;
    }

    const std::size_t openParen =
        statLine.find('(');

    const std::size_t closeParen =
        statLine.rfind(')');

    if (openParen == std::string::npos ||
        closeParen == std::string::npos ||
        closeParen <= openParen)
    {
        return false;
    }

    const std::string processFields =
        statLine.substr(closeParen + 2);

    std::istringstream stream(processFields);

    // Field 3: state
    if (!(stream >> snapshot.state))
    {
        return false;
    }

    // Field 4: parent PID
    if (!(stream >> snapshot.ppid))
    {
        return false;
    }

    // Skip fields 5 through 13.
    unsigned long long ignored = 0;

    for (int field = 5; field <= 13; ++field)
    {
        if (!(stream >> ignored))
        {
            return false;
        }
    }

    // Field 14: utime
    // Field 15: stime
    unsigned long long utime = 0;
    unsigned long long stime = 0;

    if (!(stream >> utime >> stime))
    {
        return false;
    }

    snapshot.cpuJiffies = utime + stime;

    // Read resident memory from /proc/<pid>/status.
    const std::string statusPath =
        "/proc/" + pidString + "/status";

    std::ifstream statusFile(statusPath);

    if (statusFile)
    {
        std::string line;

        while (std::getline(statusFile, line))
        {
            if (line.rfind("VmRSS:", 0) == 0)
            {
                std::istringstream memoryStream(
                    line.substr(6));

                memoryStream >> snapshot.memoryKb;
                break;
            }
        }
    }

    return true;
}

std::string readProcessName(
    const std::string& pidString)
{
    const std::string statPath =
        "/proc/" + pidString + "/stat";

    std::ifstream statFile(statPath);

    if (!statFile)
    {
        return {};
    }

    std::string statLine;

    if (!std::getline(statFile, statLine))
    {
        return {};
    }

    const std::size_t openParen =
        statLine.find('(');

    const std::size_t closeParen =
        statLine.rfind(')');

    if (openParen == std::string::npos ||
        closeParen == std::string::npos ||
        closeParen <= openParen)
    {
        return {};
    }

    return statLine.substr(
        openParen + 1,
        closeParen - openParen - 1);
}

} // namespace

std::vector<ProcessInfo> ProcessMonitor::getProcesses()
{
    std::vector<ProcessInfo> processes;

    DIR* procDir = opendir("/proc");

    if (procDir == nullptr)
    {
        return processes;
    }

    std::unordered_map<int, unsigned long long>
        initialCpuJiffies;

    struct dirent* entry = nullptr;

    // First sample.
    while ((entry = readdir(procDir)) != nullptr)
    {
        const std::string pidString = entry->d_name;

        if (!isNumeric(pidString))
        {
            continue;
        }

        int pid = 0;

        try
        {
            pid = std::stoi(pidString);
        }
        catch (...)
        {
            continue;
        }

        ProcessSnapshot snapshot;

        if (!readProcessSnapshot(pidString, snapshot))
        {
            continue;
        }

        ProcessInfo info{};

        info.pid = pid;
        info.ppid = snapshot.ppid;
        info.name = readProcessName(pidString);
        info.state = std::string(1, snapshot.state);
        info.memoryKb = snapshot.memoryKb;
        info.cpuUsage = 0.0;

        initialCpuJiffies[pid] =
            snapshot.cpuJiffies;

        processes.push_back(info);
    }

    closedir(procDir);

    // Measure CPU usage over 100 ms.
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    const long clockTicks =
        sysconf(_SC_CLK_TCK);

    if (clockTicks <= 0)
    {
        return processes;
    }

    // Second sample.
    for (auto& process : processes)
    {
        const std::string pidString =
            std::to_string(process.pid);

        ProcessSnapshot snapshot;

        if (!readProcessSnapshot(
                pidString,
                snapshot))
        {
            continue;
        }

        const auto initial =
            initialCpuJiffies.find(process.pid);

        if (initial == initialCpuJiffies.end())
        {
            continue;
        }

        if (snapshot.cpuJiffies < initial->second)
        {
            continue;
        }

        const unsigned long long deltaJiffies =
            snapshot.cpuJiffies - initial->second;

        /*
         * CPU percentage over a 100 ms interval:
         *
         * 100% CPU = 100 ms of CPU time
         *
         * One jiffy = 1 / clockTicks seconds.
         */
        const double cpuSeconds =
            static_cast<double>(deltaJiffies) /
            static_cast<double>(clockTicks);

        const double elapsedSeconds = 0.1;

        process.cpuUsage =
            (cpuSeconds / elapsedSeconds) * 100.0;

        if (process.cpuUsage < 0.0)
        {
            process.cpuUsage = 0.0;
        }
    }

    return processes;
}
