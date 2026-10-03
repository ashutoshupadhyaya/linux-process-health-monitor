#include "configuration/configuration_manager.hpp"

#include <fstream>
#include <string>

ConfigurationManager::ConfigurationManager()
    : cpuWarningThreshold(70.0),
      cpuCriticalThreshold(90.0),
      memoryWarningThreshold(250000),
      memoryCriticalThreshold(500000),
      monitorIntervalMs(100),
      configPath("config/monitor.conf")
{
    loadConfiguration();
    validateConfiguration();
}

void ConfigurationManager::loadConfiguration()
{
    std::ifstream file(configPath);

    if (!file)
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::size_t separator = line.find('=');

        if (separator == std::string::npos)
        {
            continue;
        }

        std::string key = line.substr(0, separator);
        std::string value = line.substr(separator + 1);

        try
        {
            if (key == "CPU_WARNING")
            {
                cpuWarningThreshold = std::stod(value);
            }
            else if (key == "CPU_CRITICAL")
            {
                cpuCriticalThreshold = std::stod(value);
            }
            else if (key == "MEMORY_WARNING")
            {
                memoryWarningThreshold = std::stol(value);
            }
            else if (key == "MEMORY_CRITICAL")
            {
                memoryCriticalThreshold = std::stol(value);
            }
            else if (key == "MONITOR_INTERVAL_MS")
            {
                monitorIntervalMs = std::stoi(value);
            }
        }
        catch (...)
        {
            // Keep the existing default value.
        }
    }
}

void ConfigurationManager::validateConfiguration()
{
    if (cpuWarningThreshold < 0.0 ||
        cpuWarningThreshold > 100.0)
    {
        cpuWarningThreshold = 70.0;
    }

    if (cpuCriticalThreshold < 0.0 ||
        cpuCriticalThreshold > 100.0)
    {
        cpuCriticalThreshold = 90.0;
    }

    if (cpuCriticalThreshold < cpuWarningThreshold)
    {
        cpuCriticalThreshold = cpuWarningThreshold;
    }

    if (memoryWarningThreshold < 0)
    {
        memoryWarningThreshold = 250000;
    }

    if (memoryCriticalThreshold < 0)
    {
        memoryCriticalThreshold = 500000;
    }

    if (memoryCriticalThreshold < memoryWarningThreshold)
    {
        memoryCriticalThreshold = memoryWarningThreshold;
    }

    if (monitorIntervalMs <= 0)
    {
        monitorIntervalMs = 100;
    }
}

double ConfigurationManager::getCpuWarningThreshold() const
{
    return cpuWarningThreshold;
}

double ConfigurationManager::getCpuCriticalThreshold() const
{
    return cpuCriticalThreshold;
}

long ConfigurationManager::getMemoryWarningThreshold() const
{
    return memoryWarningThreshold;
}

long ConfigurationManager::getMemoryCriticalThreshold() const
{
    return memoryCriticalThreshold;
}

int ConfigurationManager::getMonitorIntervalMs() const
{
    return monitorIntervalMs;
}
