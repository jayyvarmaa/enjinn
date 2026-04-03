#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>
#include "gizmos.h"
#include <sceneGraph/node.h>
#include <glm/glm.hpp>

namespace enjinn
{
    struct SceneViewWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_PICTURE_O " Scene";
        
        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(unsigned int textureId, bool &open);
        
        // Gizmo integration
        void setSelectedNode(Node* node) { selectedNode = node; }
        void setCameraMatrices(const glm::mat4& view, const glm::mat4& projection) 
        { 
            viewMatrix = view; 
            projectionMatrix = projection; 
        }
        
        TransformGizmo& getGizmo() { return gizmo; }
        
        // Check if gizmo is being used (to prevent camera movement)
        bool isGizmoActive() const { return gizmoActive; }

    private:
        int windowId = 0;
        
        // Gizmo system
        TransformGizmo gizmo;
        Node* selectedNode = nullptr;
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        bool gizmoActive = false;
        
        // Mouse state
        glm::vec2 lastMousePos = glm::vec2(0.0f);
        bool wasMouseDown = false;
    };
}
