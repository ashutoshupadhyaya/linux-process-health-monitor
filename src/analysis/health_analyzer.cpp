#include "health_analyzer.hpp"

HealthResult HealthAnalyzer::analyzeProcess(const ProcessInfo& process)
{
    ConfigurationManager config;

    HealthResult result;

    result.pid = process.pid;
    result.name = process.name;

    if (process.cpuUsage >= config.getCpuCriticalThreshold() ||
        process.memoryKb >= config.getMemoryCriticalThreshold())
    {
        result.status = HealthStatus::CRITICAL;
        result.reason = "High CPU or memory usage";
    }
    else if (process.cpuUsage >= config.getCpuWarningThreshold() ||
             process.memoryKb >= config.getMemoryWarningThreshold())
    {
        result.status = HealthStatus::WARNING;
        result.reason = "Elevated CPU or memory usage";
    }
    else
    {
        result.status = HealthStatus::NORMAL;
        result.reason = "Resource usage within limits";
    }

    return result;
}
