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
    static PhaseManager& Get();

    bool IsFeatureUnlocked(DevelopmentPhase phase);
    void SetPhase(DevelopmentPhase phase);
    DevelopmentPhase GetCurrentPhase() const;
    bool TryUnlockPhaseSelector(const std::string& secret);
    bool IsPhaseSelectorUnlocked() const;
    void SavePhaseState();
    void LoadPhaseState();
    const char* GetPhaseName() const;
    const char* GetPhaseName(DevelopmentPhase phase) const;

private:
    DevelopmentPhase currentPhase = DevelopmentPhase::Phase5_Complete;
    bool phaseSelectorUnlocked = false; // Renamed from selectorUnlocked to match cpp
};

}
