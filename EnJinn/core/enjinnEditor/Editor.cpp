#include "Editor.h"
#include "phaseManager/phaseManager.h"
#include <cstring>
#include <iostream>
#include <IconsFontAwesome6.h>

namespace enjinn {
    void Editor::update(float deltaTime, const EditorContext& context) {
        
        // Usage from context
        PhaseManager* phaseMgr = context.phaseManager;
        
        // Dummy logic using params from context
        
        // Secret code logic
        if (phaseMgr && phaseMgr->TryUnlockPhaseSelector(secretCodeBuffer)) {
             context.logs->log("Unlocked!");
             context.pushNotificationManager->pushNotification("Unlocked");
        }
    }
}
