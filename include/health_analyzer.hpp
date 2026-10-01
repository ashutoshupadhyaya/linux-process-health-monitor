#include "configuration_manager.hpp"
#ifndef HEALTH_ANALYZER_HPP
#define HEALTH_ANALYZER_HPP

#include "process_monitor.hpp"
#include <string>

enum class HealthStatus
{
    NORMAL,
    WARNING,
    CRITICAL
};

struct HealthResult
{
    int pid;
    std::string name;
    HealthStatus status;
    std::string reason;
};

class HealthAnalyzer
{
public:
    HealthResult analyzeProcess(const ProcessInfo& process);
};

#endif

