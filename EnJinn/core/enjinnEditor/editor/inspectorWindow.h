#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>
#include <sceneGraph/node.h>
#include "IconsFontAwesome6.h"

namespace enjinn
{
    struct InspectorWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_INFO " Inspector";
        
        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(Node* selectedNode, bool &open);

    private:
        int windowId = 0;
        
        void drawTransformSection(Node* node);
        void drawComponentSection(Node* node);
        void drawAddComponentMenu(Node* node);
        
        const char* getComponentIcon(Component* component);
        
        Component* componentToRemove = nullptr;
    };
}
