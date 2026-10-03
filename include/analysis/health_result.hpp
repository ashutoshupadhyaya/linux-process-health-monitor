#ifndef HEALTH_RESULT_HPP
#define HEALTH_RESULT_HPP

#include <chrono>
#include <string>

enum class HealthStatus
{
    NORMAL,
    WARNING,
    CRITICAL
};

struct HealthResult
{
    HealthStatus status;
    int pid;
    std::string processName;
    double currentValue;
    double threshold;
    std::chrono::system_clock::time_point timestamp;
    std::string description;
};

#endif
