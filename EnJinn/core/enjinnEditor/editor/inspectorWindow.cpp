#include "inspectorWindow.h"
#include <imgui.h>
#include <vector>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "IconsFontAwesome6.h"

// Include Generic Components
#include <sceneGraph/components/spriteComponent.h>
#include <sceneGraph/components/physicsComponent.h>

namespace enjinn
{
    void InspectorWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
    {
        windowId = idManager.getImguiIds(1);
    }

    void InspectorWindow::update(Node* selectedNode, bool &open)
    {
        // Enforce visibility
        ImGui::PushID(windowId);
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
        if(ImGui::Begin(ICON_NAME, nullptr)) // Pass nullptr to remove close button
        {
            if(selectedNode)
            {
                // Header: Active, Name
                ImGui::Checkbox("##Active", &selectedNode->active);
                ImGui::SameLine();
                char buffer[256];
                strncpy(buffer, selectedNode->name.c_str(), sizeof(buffer));
                if(ImGui::InputText("Name", buffer, sizeof(buffer)))
                {
                    selectedNode->name = buffer;
                }

                // Tag and Layer
                char tagBuffer[256];
                strncpy(tagBuffer, selectedNode->tag.c_str(), sizeof(tagBuffer));
                if(ImGui::InputText("Tag", tagBuffer, sizeof(tagBuffer)))
                {
                    selectedNode->tag = tagBuffer;
                }

                ImGui::SameLine();
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("Layer", &selectedNode->layer);

                ImGui::Separator();
                
                // Transform
                ImGui::TextColored(ImVec4(1,1,0,1), "Transform");
                ImGui::DragFloat3("Position", &selectedNode->localPosition.x, 0.1f);
                glm::vec3 euler = glm::degrees(glm::eulerAngles(selectedNode->localRotation));
                if(ImGui::DragFloat3("Rotation", &euler.x, 0.1f))
                {
                    selectedNode->localRotation = glm::quat(glm::radians(euler));
                }
                ImGui::DragFloat3("Scale", &selectedNode->localScale.x, 0.1f);
                
                selectedNode->updateTransforms();

                ImGui::Separator();
                
                // Components
                for(size_t i=0; i<selectedNode->components.size(); ++i)
                {
                    auto* component = selectedNode->components[i];
                    ImGui::PushID(component);
                    // Simple foldout for components
                    if(ImGui::CollapsingHeader("Component", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        component->onInspector();
                    }
                    ImGui::PopID();
                    ImGui::Separator();
                }

                // Add Component Button
                if(ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    ImGui::OpenPopup("AddComponentPopup");
                }

                if(ImGui::BeginPopup("AddComponentPopup"))
                {
                    if(ImGui::MenuItem("Sprite Component"))
                    {
                        selectedNode->addComponent<GenericSpriteComponent>();
                    }
                    if(ImGui::MenuItem("Physics Component (Beta)"))
                    {
                        selectedNode->addComponent<GenericPhysicsComponent>();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
        ImGui::PopID();
    }
}
