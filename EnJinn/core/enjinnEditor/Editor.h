#pragma once
#include <vector>
#include <string>

class ShortcutManager {};
class LogManager { public: void log(const char* msg) {} };
class ContainerManager {};
class ImguiIDsManager {};
class ConsoleWindow {};
class PushNotificationManager { public: void pushNotification(const std::string& msg) {} };

namespace enjinn {
    class PhaseManager;

    struct EditorContext {
        ShortcutManager* shortcutManager;
        LogManager* logs;
        ContainerManager* containerManager;
        ImguiIDsManager* imguiIDsManager;
        ConsoleWindow* consoleWindow;
        PushNotificationManager* pushNotificationManager;
        PhaseManager* phaseManager;
    };

    class Editor {
    public:
        void update(float deltaTime, const EditorContext& context);
    
    private:
        char secretCodeBuffer[64] = {};
    };
}
