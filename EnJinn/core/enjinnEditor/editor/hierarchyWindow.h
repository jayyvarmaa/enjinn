#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>
#include <sceneGraph/scene.h> 
#include "IconsFontAwesome6.h" 
#include <vector>
#include <unordered_map>

namespace enjinn
{
    struct HierarchyWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_LIST " Hierarchy";
        
        Node* selectedNode = nullptr;
        std::vector<Node*> selectedNodes; // Multi-selection support
        char searchBuffer[256] = {};

        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(Scene* scene, bool &open);
        
        // Node visibility and locking
        bool isNodeVisible(Node* node) const;
        bool isNodeLocked(Node* node) const;
        void setNodeVisible(Node* node, bool visible);
        void setNodeLocked(Node* node, bool locked);
        
    private:
        int windowId = 0;
        void drawNode(Node* node, int depth = 0);
        void drawContextMenu(Node* node);
        const char* getNodeIcon(Node* node);
        
        Node* nodeToDestroy = nullptr;
        Node* dragSource = nullptr;
        Node* dragTarget = nullptr;
        Node* contextMenuNode = nullptr;
        
        // Node states
        std::unordered_map<Node*, bool> nodeVisibility;
        std::unordered_map<Node*, bool> nodeLocked;
        std::unordered_map<Node*, bool> nodeExpanded;
        
        // Copy/paste buffer
        Node* copiedNode = nullptr;
        
        // Rename mode
        Node* renamingNode = nullptr;
        char renameBuffer[256] = {};
    };
}
