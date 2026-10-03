#include "recovery/signal_manager.hpp"

#include <cerrno>
#include <csignal>
#include <iostream>

bool SignalManager::sendTerminateSignal(int pid)
{
    if (pid <= 0)
    {
        std::cerr << "[SIGNAL] Invalid PID." << std::endl;
        return false;
    }

    if (kill(pid, SIGTERM) != 0)
    {
        std::cerr
            << "[SIGNAL] Failed to send signal to PID "
            << pid
            << " (errno "
            << errno
            << ")."
            << std::endl;

        return false;
    }

    std::cout
        << "[SIGNAL] Signal 15 sent to PID "
        << pid
        << "."
        << std::endl;

    return true;
}
