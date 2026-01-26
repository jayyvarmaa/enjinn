#pragma once
#include <enjinnConfig.h>
#include <string>
#include <vector>

namespace enjinn {

enum class DevelopmentPhase {
    Phase1_Foundation,
    Phase2_CoreSystems,
    Phase3_Editor,
    Phase4_PhysicsGameplay,
    Phase5_Complete
};

class PhaseManager {
public:
    PhaseManager() = default;

    bool IsFeatureUnlocked(DevelopmentPhase phase) {
        return currentPhase >= phase;
    }

    void SetPhase(DevelopmentPhase phase) {
        currentPhase = phase;
    }

    DevelopmentPhase GetCurrentPhase() const {
        return currentPhase;
    }

    bool TryUnlockPhaseSelector(const std::string& secret) {
        if (secret == PHASE_UNLOCK_SECRET) {
            selectorUnlocked = true;
            return true;
        }
        return false;
    }

    bool IsPhaseSelectorUnlocked() const { return selectorUnlocked; }

private:
    DevelopmentPhase currentPhase = DevelopmentPhase::Phase5_Complete;
    bool selectorUnlocked = false;
};

}
