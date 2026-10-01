#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

class ConfigurationManager
{
private:
    double cpuWarningThreshold;
    double cpuCriticalThreshold;

    long long memoryWarningThreshold;
    long long memoryCriticalThreshold;

    int monitoringIntervalMs;

public:
    ConfigurationManager();

    double getCpuWarningThreshold() const;
    double getCpuCriticalThreshold() const;

    long long getMemoryWarningThreshold() const;
    long long getMemoryCriticalThreshold() const;

    int getMonitoringIntervalMs() const;

};

#endif
