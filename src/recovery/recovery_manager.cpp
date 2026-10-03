#include "recovery/recovery_manager.hpp"

#include <unistd.h>

#include <iostream>

bool RecoveryManager::handleRecovery(
    const HealthResult& result)
{
    if (result.status != HealthStatus::CRITICAL)
    {
        return false;
    }

    if (result.pid <= 0)
    {
        std::cerr
            << "[RECOVERY] Invalid PID."
            << std::endl;

        return false;
    }

    if (result.pid == 1)
    {
        std::cerr
            << "[RECOVERY] Refusing to terminate PID 1."
            << std::endl;

        return false;
    }

    if (result.pid == static_cast<int>(getpid()))
    {
        std::cerr
            << "[RECOVERY] Refusing to terminate monitor process."
            << std::endl;

        return false;
    }

    std::cout
        << "[RECOVERY] Critical condition detected for "
        << result.processName
        << " (PID "
        << result.pid
        << ")."
        << std::endl;

    bool success =
        signalManager.sendTerminateSignal(result.pid);

    if (success)
    {
        std::cout
            << "[RECOVERY] Recovery signal sent successfully."
            << std::endl;
    }
    else
    {
        std::cerr
            << "[RECOVERY] Failed to send recovery signal."
            << std::endl;
    }

    return success;
}
