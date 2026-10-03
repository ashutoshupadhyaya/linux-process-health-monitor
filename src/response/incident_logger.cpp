#include "response/incident_logger.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <iostream>

void IncidentLogger::logIncident(
    const HealthResult& result)
{
    if (result.status == HealthStatus::NORMAL)
    {
        return;
    }

    std::filesystem::create_directories("logs");

    std::ofstream logFile(
        "logs/incidents.log",
        std::ios::app);

    if (!logFile)
    {
        std::cerr
            << "[LOGGER] Failed to open "
            << "logs/incidents.log"
            << std::endl;

        return;
    }

    const std::time_t timeValue =
        std::chrono::system_clock::to_time_t(
            result.timestamp);

    std::tm timeInfo{};

    localtime_r(&timeValue, &timeInfo);

    logFile << "["
            << std::put_time(
                &timeInfo,
                "%Y-%m-%d %H:%M:%S")
            << "] ";

    if (result.status == HealthStatus::WARNING)
    {
        logFile << "Status: WARNING";
    }
    else
    {
        logFile << "Status: CRITICAL";
    }

    logFile << " | PID: "
            << result.pid
            << " | Process: "
            << result.processName
            << " | Description: "
            << result.description
            << '\n';
}
