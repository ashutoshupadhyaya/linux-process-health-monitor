#ifndef RECOVERY_VERIFIER_HPP
#define RECOVERY_VERIFIER_HPP

#include "analysis/health_result.hpp"

class RecoveryVerifier
{
public:
    bool verifyRecovery(
        const HealthResult& result,
        bool recoverySucceeded);
};

#endif
