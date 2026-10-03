#include "analysis/health_analyzer.hpp"

HealthResult HealthAnalyzer::analyzeProcess(
    const ProcessInfo& process)
{
    ConfigurationManager config;

    HealthResult result{};

    result.pid = process.pid;
    result.processName = process.name;
    result.timestamp =
        std::chrono::system_clock::now();

    if (process.cpuUsage >=
            config.getCpuCriticalThreshold() ||
        process.memoryKb >=
            config.getMemoryCriticalThreshold())
    {
        result.status = HealthStatus::CRITICAL;

        if (process.cpuUsage >=
            config.getCpuCriticalThreshold())
        {
            result.currentValue =
                process.cpuUsage;

            result.threshold =
                config.getCpuCriticalThreshold();
        }
        else
        {
            result.currentValue =
                static_cast<double>(
                    process.memoryKb);

            result.threshold =
                static_cast<double>(
                    config.getMemoryCriticalThreshold());
        }

        result.description =
            "High CPU or memory usage";
    }
    else if (
        process.cpuUsage >=
            config.getCpuWarningThreshold() ||
        process.memoryKb >=
            config.getMemoryWarningThreshold())
    {
        result.status = HealthStatus::WARNING;

        if (process.cpuUsage >=
            config.getCpuWarningThreshold())
        {
            result.currentValue =
                process.cpuUsage;

            result.threshold =
                config.getCpuWarningThreshold();
        }
        else
        {
            result.currentValue =
                static_cast<double>(
                    process.memoryKb);

            result.threshold =
                static_cast<double>(
                    config.getMemoryWarningThreshold());
        }

        result.description =
            "Elevated CPU or memory usage";
    }
    else
    {
        result.status = HealthStatus::NORMAL;
        result.currentValue = process.cpuUsage;
        result.threshold =
            config.getCpuWarningThreshold();

        result.description =
            "Resource usage within limits";
    }

    return result;
}
