#include "configuration_manager.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <exception>

ConfigurationManager::ConfigurationManager()
{
    // Default configuration values
    cpuWarningThreshold = 70.0;
    cpuCriticalThreshold = 90.0;

    memoryWarningThreshold = 250000;
    memoryCriticalThreshold = 500000;

    monitoringIntervalMs = 100;

    std::ifstream file("config/monitor.conf");

    if (!file.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream stream(line);

        std::string key;
        std::string value;

        if (std::getline(stream, key, '=') &&
            std::getline(stream, value))
        {
            if (key == "CPU_WARNING")
            {
                try
                {
                    cpuWarningThreshold = std::stod(value);
                }
                catch (const std::exception&)
                {
                    // Keep the default value
                }
            }
            else if (key == "CPU_CRITICAL")
            {
                try
                {
                    cpuCriticalThreshold = std::stod(value);
                }
                catch (const std::exception&)
                {
                    // Keep the default value
                }
            }
            else if (key == "MEMORY_WARNING")
            {
                try
                {
                    memoryWarningThreshold = std::stoll(value);
                }
                catch (const std::exception&)
                {
                    // Keep the default value
                }
            }
            else if (key == "MEMORY_CRITICAL")
            {
                try
                {
                    memoryCriticalThreshold = std::stoll(value);
                }
                catch (const std::exception&)
                {
                    // Keep the default value
                }
            }
            else if (key == "MONITOR_INTERVAL_MS")
            {
                try
                {
                    monitoringIntervalMs = std::stoi(value);
                }
                catch (const std::exception&)
                {
                    // Keep the default value
                }
            }
        }
    }

    // Validate configuration values

    if (cpuWarningThreshold < 0.0)
    {
        cpuWarningThreshold = 0.0;
    }

    if (cpuCriticalThreshold <= cpuWarningThreshold)
    {
        cpuCriticalThreshold = cpuWarningThreshold + 20.0;
    }

    if (memoryWarningThreshold < 0)
    {
        memoryWarningThreshold = 0;
    }

    if (memoryCriticalThreshold <= memoryWarningThreshold)
    {
        memoryCriticalThreshold = memoryWarningThreshold + 250000;
    }

    if (monitoringIntervalMs <= 0)
    {
        monitoringIntervalMs = 100;
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

long long ConfigurationManager::getMemoryWarningThreshold() const
{
    return memoryWarningThreshold;
}

long long ConfigurationManager::getMemoryCriticalThreshold() const
{
    return memoryCriticalThreshold;
}

int ConfigurationManager::getMonitoringIntervalMs() const
{
    return monitoringIntervalMs;
}
