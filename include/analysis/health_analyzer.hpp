#ifndef HEALTH_ANALYZER_HPP
#define HEALTH_ANALYZER_HPP

#include "analysis/health_result.hpp"
#include "configuration/configuration_manager.hpp"
#include "monitoring/process_monitor.hpp"

class HealthAnalyzer
{
public:
    HealthResult analyzeProcess(
        const ProcessInfo& process);
};

#endif
