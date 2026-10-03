#ifndef RECOVERY_MANAGER_HPP
#define RECOVERY_MANAGER_HPP

#include "analysis/health_result.hpp"
#include "recovery/signal_manager.hpp"

class RecoveryManager
{
public:
    bool handleRecovery(
        const HealthResult& result);

private:
    SignalManager signalManager;
};

#endif
