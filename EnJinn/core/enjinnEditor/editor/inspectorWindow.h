#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>
#include <sceneGraph/node.h>

namespace enjinn
{
    struct InspectorWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_INFO " Inspector";
        
        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(Node* selectedNode, bool &open);

    private:
        int windowId = 0;
    };
}
