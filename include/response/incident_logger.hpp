#ifndef INCIDENT_LOGGER_HPP
#define INCIDENT_LOGGER_HPP

#include "analysis/health_result.hpp"

class IncidentLogger
{
public:
    void logIncident(const HealthResult& result);
};

#endif
