#include "recovery/recovery_verifier.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

bool RecoveryVerifier::verifyRecovery(
    const HealthResult& result,
    bool recoverySucceeded)
{
    if (!recoverySucceeded)
    {
        std::cout
            << "[VERIFICATION] Recovery was not successful."
            << std::endl;

        return false;
    }

    const std::string procPath =
        "/proc/" + std::to_string(result.pid);

    constexpr int maxAttempts = 20;
    constexpr int waitMilliseconds = 100;

    for (int attempt = 0;
         attempt < maxAttempts;
         ++attempt)
    {
        if (!std::filesystem::exists(procPath))
        {
            std::cout
                << "[VERIFICATION] Process "
                << result.processName
                << " (PID "
                << result.pid
                << ") is no longer running."
                << std::endl;

            return true;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                waitMilliseconds));
    }

    std::cout
        << "[VERIFICATION] Process "
        << result.processName
        << " (PID "
        << result.pid
        << ") is still running."
        << std::endl;

    return false;
}
