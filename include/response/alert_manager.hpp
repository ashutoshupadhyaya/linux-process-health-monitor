#ifndef ALERT_MANAGER_HPP
#define ALERT_MANAGER_HPP

#include "analysis/health_result.hpp"

class AlertManager
{
public:
    void handleHealthResult(const HealthResult& result);
};

#endif
