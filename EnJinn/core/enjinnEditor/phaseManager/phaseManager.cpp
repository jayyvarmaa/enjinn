//////////////////////////////////////////
// phaseManager.cpp
// EnJinn Engine - Development Phase Manager
//////////////////////////////////////////

#include "phaseManager.h"
#include <safeSave/safeSave.h>
#include <cstring>
#include <filesystem>

#define SECRET_CODE "letmein"
#define PHASE_SAVE_PATH ENJINN_ENGINE_SAVES_PATH "phase.dat"

namespace enjinn
{
    // Singleton Accessor
    PhaseManager& PhaseManager::Get()
    {
        static PhaseManager instance;
        return instance;
    }

    // Default constructor not needed if using singleton access only, but kept for compatibility
    // PhaseManager::PhaseManager() { LoadPhaseState(); } // Removed explicit ctor since we use default in header or singleton

    bool PhaseManager::IsFeatureUnlocked(DevelopmentPhase phase)
    {
        return currentPhase >= phase;
    }

    void PhaseManager::SetPhase(DevelopmentPhase phase)
    {
        currentPhase = phase;
        SavePhaseState();
    }
    
    DevelopmentPhase PhaseManager::GetCurrentPhase() const
    {
        return currentPhase;
    }

    bool PhaseManager::TryUnlockPhaseSelector(const std::string& code)
    {
        if (code == SECRET_CODE)
        {
            phaseSelectorUnlocked = true;
            SavePhaseState();
            return true;
        }
        return false;
    }

    bool PhaseManager::IsPhaseSelectorUnlocked() const
    {
        return phaseSelectorUnlocked;
    }
    
    void PhaseManager::SavePhaseState()
    {
        struct PhaseData
        {
            int phase;
            bool unlocked;
        };
        
        PhaseData data;
        data.phase = static_cast<int>(currentPhase);
        data.unlocked = phaseSelectorUnlocked;
        
        sfs::safeSave(&data, sizeof(data), PHASE_SAVE_PATH, false);
    }
    
    void PhaseManager::LoadPhaseState()
    {
        struct PhaseData
        {
            int phase;
            bool unlocked;
        };
        
        PhaseData data;
        
        if (sfs::safeLoad(&data, sizeof(data), PHASE_SAVE_PATH, false) == sfs::noError)
        {
            currentPhase = static_cast<DevelopmentPhase>(data.phase);
            phaseSelectorUnlocked = data.unlocked;
        }
        else
        {
            // Default: All features unlocked (normal development mode)
            currentPhase = DevelopmentPhase::Phase5_Complete;
            phaseSelectorUnlocked = false;
        }
    }
    
    const char* PhaseManager::GetPhaseName() const
    {
        return GetPhaseName(currentPhase);
    }
    
    const char* PhaseManager::GetPhaseName(DevelopmentPhase phase) const
    {
        switch (phase)
        {
        case DevelopmentPhase::Phase1_Foundation:
            return "Phase 1: Foundation";
        case DevelopmentPhase::Phase2_CoreSystems:
            return "Phase 2: Core Systems";
        case DevelopmentPhase::Phase3_Editor:
            return "Phase 3: Editor Suite";
        case DevelopmentPhase::Phase4_PhysicsGameplay:
            return "Phase 4: Physics & Gameplay";
        case DevelopmentPhase::Phase5_Complete:
            return "Phase 5: Complete";
        default:
            return "Unknown Phase";
        }
    }
}
