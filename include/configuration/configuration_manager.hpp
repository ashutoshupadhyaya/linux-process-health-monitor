#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

#include <string>

class ConfigurationManager
{
public:
    ConfigurationManager();

    double getCpuWarningThreshold() const;
    double getCpuCriticalThreshold() const;

    long getMemoryWarningThreshold() const;
    long getMemoryCriticalThreshold() const;

    int getMonitorIntervalMs() const;

private:
    double cpuWarningThreshold;
    double cpuCriticalThreshold;

    long memoryWarningThreshold;
    long memoryCriticalThreshold;

    int monitorIntervalMs;

    void loadConfiguration();
    void validateConfiguration();

    std::string configPath;
};

#endif
