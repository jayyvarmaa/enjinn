#include "core/enjinnEditor/Editor.h"
#include "core/enjinnEditor/phaseManager/phaseManager.h"

int main() {
    enjinn::Editor editor;
    ShortcutManager sm;
    LogManager lm;
    ContainerManager cm;
    ImguiIDsManager im;
    ConsoleWindow cw;
    PushNotificationManager pm;
    enjinn::PhaseManager phaseMgr;

    enjinn::EditorContext context;
    context.shortcutManager = &sm;
    context.logs = &lm;
    context.containerManager = &cm;
    context.imguiIDsManager = &im;
    context.consoleWindow = &cw;
    context.pushNotificationManager = &pm;
    context.phaseManager = &phaseMgr;

    editor.update(0.016f, context);
    return 0;
}
