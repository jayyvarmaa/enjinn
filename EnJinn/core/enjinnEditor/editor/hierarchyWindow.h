#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>
#include <sceneGraph/scene.h> 
#include "IconsFontAwesome6.h" 

namespace enjinn
{
    struct HierarchyWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_LIST " Hierarchy";
        
        Node* selectedNode = nullptr;
        char searchBuffer[256] = {};

        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(Scene* scene, bool &open);
        
    private:
        int windowId = 0;
        void drawNode(Node* node);
        
        Node* nodeToDestroy = nullptr;
        Node* dragSource = nullptr;
        Node* dragTarget = nullptr;
    };
}
