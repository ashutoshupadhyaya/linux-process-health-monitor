#include "response/alert_manager.hpp"

#include <iostream>

void AlertManager::handleHealthResult(
    const HealthResult& result)
{
    if (result.status == HealthStatus::NORMAL)
    {
        std::cout
            << "[NORMAL] "
            << result.processName
            << " (PID " << result.pid << ")"
            << " is healthy."
            << std::endl;
    }
    else if (result.status == HealthStatus::WARNING)
    {
        std::cout
            << "[WARNING] "
            << result.processName
            << " (PID " << result.pid << ")"
            << " requires attention."
            << std::endl;
    }
    else if (result.status == HealthStatus::CRITICAL)
    {
        std::cout
            << "[CRITICAL] "
            << result.processName
            << " (PID " << result.pid << ")"
            << " requires immediate attention."
            << std::endl;
    }
}
