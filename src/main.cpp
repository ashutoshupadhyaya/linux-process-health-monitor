#include <iostream>
#include <string>

#include "monitoring/system_monitor.hpp"
#include "monitoring/process_monitor.hpp"

#include "configuration/configuration_manager.hpp"

#include "analysis/health_analyzer.hpp"
#include "analysis/health_result.hpp"

#include "response/alert_manager.hpp"
#include "response/incident_logger.hpp"

#include "recovery/recovery_manager.hpp"
#include "recovery/recovery_verifier.hpp"

#include "device/device_interface_manager.hpp"

int main()
{
    SystemMonitor systemMonitor;
    ProcessMonitor processMonitor;
    ConfigurationManager configurationManager;
    HealthAnalyzer healthAnalyzer;

    AlertManager alertManager;
    IncidentLogger incidentLogger;

    RecoveryManager recoveryManager;
    RecoveryVerifier recoveryVerifier;

    DeviceInterfaceManager deviceInterface;

    std::cout
        << "Linux Process Health Monitor started."
        << std::endl;

    std::cout
        << "CPU Usage: "
        << systemMonitor.getCpuUsage()
        << "%"
        << std::endl;

    std::cout
        << "Memory Usage: "
        << systemMonitor.getMemoryUsage()
        << "%"
        << std::endl;

    std::cout
        << "Disk Usage: "
        << systemMonitor.getDiskUsage()
        << "%"
        << std::endl;

    std::cout
        << "\nRunning Processes:\n";

    const auto processes =
        processMonitor.getProcesses();

    int count = 0;

    for (const auto& process : processes)
    {
        HealthResult result =
            healthAnalyzer.analyzeProcess(process);

        std::cout
            << "PID: "
            << process.pid
            << " | PPID: "
            << process.ppid
            << " | Name: "
            << process.name
            << " | State: "
            << process.state
            << " | Memory: "
            << process.memoryKb
            << " kB"
            << " | CPU: "
            << process.cpuUsage
            << "%"
            << std::endl;

        std::cout << "   Health: ";

        if (result.status == HealthStatus::NORMAL)
        {
            std::cout << "NORMAL";
        }
        else if (result.status == HealthStatus::WARNING)
        {
            std::cout << "WARNING";
        }
        else
        {
            std::cout << "CRITICAL";
        }

        std::cout
            << " - "
            << result.description
            << std::endl;

        alertManager.handleHealthResult(result);

        if (result.status == HealthStatus::WARNING ||
            result.status == HealthStatus::CRITICAL)
        {
            incidentLogger.logIncident(result);
        }

        if (result.status == HealthStatus::CRITICAL)
        {
            bool recoverySucceeded =
                recoveryManager.handleRecovery(result);

            bool recoveryVerified =
                recoveryVerifier.verifyRecovery(
                    result,
                    recoverySucceeded);

            if (recoveryVerified)
            {
                if (deviceInterface.openDevice())
                {
                    std::string message =
                        "Critical incident handled for " +
                        result.processName;

                    if (deviceInterface.writeMessage(message))
                    {
                        std::string driverResponse;

                        if (deviceInterface.readMessage(
                                driverResponse))
                        {
                            std::cout
                                << "[DEVICE] Driver response: "
                                << driverResponse
                                << std::endl;
                        }
                    }

                    deviceInterface.closeDevice();
                }
            }

            std::cout
                << "[RESULT] Recovery verified: "
                << (recoveryVerified ? "YES" : "NO")
                << std::endl;
        }

        ++count;

        if (count >= 10)
        {
            break;
        }
    }

    return 0;
}
