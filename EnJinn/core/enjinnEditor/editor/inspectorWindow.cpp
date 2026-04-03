#include "inspectorWindow.h"
#include <imgui.h>
#include <vector>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// Include Generic Components
#include <sceneGraph/components/spriteComponent.h>
#include <sceneGraph/components/physicsComponent.h>
#include "../enjinnRuntime/audio/audioComponents.h"
#include "../enjinnRuntime/rendering/cameraComponent.h"
#include "../enjinnRuntime/rendering/lightComponent.h"
#include "../enjinnRuntime/rendering/meshRendererComponent.h"
#include "../enjinnRuntime/rendering/materialComponent.h"
#include "../enjinnRuntime/scripting/scriptComponent.h"
#include "../enjinnRuntime/particles/particleSystem.h"
#include "../enjinnRuntime/animation/animationSystem.h"
#include "../enjinnRuntime/physics/physicsJoints.h"
#include "../enjinnRuntime/rendering/lodSystem.h"
#include "../sharedRuntime/sceneGraph/componentFactory.h"

namespace enjinn
{
    void InspectorWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
    {
        windowId = idManager.getImguiIds(1);
    }
    
    const char* InspectorWindow::getComponentIcon(Component* component)
    {
        if (!component) return ICON_FK_CUBE;
        
        std::string compName = component->getComponentName();
        
        if (compName.find("Camera") != std::string::npos)
            return ICON_FK_VIDEO_CAMERA;
        if (compName.find("Light") != std::string::npos)
            return ICON_FK_LIGHTBULB_O;
        if (compName.find("Sprite") != std::string::npos)
            return ICON_FK_PICTURE_O;
        if (compName.find("Physics") != std::string::npos)
            return ICON_FK_CIRCLE_O;
        if (compName.find("Audio") != std::string::npos)
            return ICON_FK_VOLUME_UP;
        if (compName.find("Script") != std::string::npos)
            return ICON_FK_FILE_CODE_O;
            
        return ICON_FK_PUZZLE_PIECE;
    }

    void InspectorWindow::update(Node* selectedNode, bool &open)
    {
        ImGui::PushID(windowId);
        ImGui::SetNextWindowSize(ImVec2(350, 600), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
        
        if(!ImGui::Begin(ICON_NAME, &open))
        {
            ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopID();
            return;
        }

        if(selectedNode)
        {
            drawTransformSection(selectedNode);
            ImGui::Spacing();
            drawComponentSection(selectedNode);
            ImGui::Spacing();
            drawAddComponentMenu(selectedNode);
        }
        else
        {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No object selected");
            ImGui::TextWrapped("Select an object in the Hierarchy to view its properties.");
        }
        
        // Handle deferred component removal
        if (componentToRemove && selectedNode)
        {
            auto it = std::find(selectedNode->components.begin(), selectedNode->components.end(), componentToRemove);
            if (it != selectedNode->components.end())
            {
                selectedNode->components.erase(it);
                delete componentToRemove;
            }
            componentToRemove = nullptr;
        }

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopID();
    }
    
    void InspectorWindow::drawTransformSection(Node* node)
    {
        // Header with larger font and background
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.35f, 0.35f, 0.4f, 1.0f));
        
        bool transformOpen = ImGui::CollapsingHeader(ICON_FK_ARROWS " Transform", ImGuiTreeNodeFlags_DefaultOpen);
        
        ImGui::PopStyleColor(3);
        
        if (transformOpen)
        {
            ImGui::Indent(8.0f);
            
            // Active checkbox and name
            ImGui::Checkbox("Active", &node->active);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Toggle GameObject active state");
            
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-1);
            char nameBuffer[256];
            strncpy(nameBuffer, node->name.c_str(), sizeof(nameBuffer));
            if(ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
            {
                node->name = nameBuffer;
            }
            
            ImGui::Spacing();
            
            // Tag and Layer on same line
            ImGui::SetNextItemWidth(150);
            char tagBuffer[128];
            strncpy(tagBuffer, node->tag.c_str(), sizeof(tagBuffer));
            if(ImGui::InputText("Tag", tagBuffer, sizeof(tagBuffer)))
            {
                node->tag = tagBuffer;
            }
            
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-1);
            ImGui::InputInt("Layer", &node->layer);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Position
            ImGui::Text(ICON_FK_ARROWS " Position");
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat3("##Position", &node->localPosition.x, 0.1f))
                node->updateTransforms();
            
            ImGui::Spacing();
            
            // Rotation
            ImGui::Text(ICON_FK_REFRESH " Rotation");
            glm::vec3 euler = glm::degrees(glm::eulerAngles(node->localRotation));
            ImGui::SetNextItemWidth(-1);
            if(ImGui::DragFloat3("##Rotation", &euler.x, 0.5f))
            {
                node->localRotation = glm::quat(glm::radians(euler));
                node->updateTransforms();
            }
            
            ImGui::Spacing();
            
            // Scale
            ImGui::Text(ICON_FK_EXPAND " Scale");
            ImGui::SetNextItemWidth(-1);
            if (ImGui::DragFloat3("##Scale", &node->localScale.x, 0.01f, 0.001f, 100.0f))
                node->updateTransforms();
            
            ImGui::Unindent(8.0f);
        }
    }
    
    void InspectorWindow::drawComponentSection(Node* node)
    {
        for(size_t i=0; i<node->components.size(); ++i)
        {
            auto* component = node->components[i];
            ImGui::PushID(component);
            
            // Component header with icon
            const char* icon = getComponentIcon(component);
            std::string headerLabel = std::string(icon) + " " + component->getComponentName();
            
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.3f, 0.5f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.35f, 0.55f, 0.9f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.3f, 0.4f, 0.6f, 1.0f));
            
            bool componentOpen = ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
            
            ImGui::PopStyleColor(3);
            
            // Context menu for component
            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem(ICON_FK_TRASH " Remove Component"))
                {
                    componentToRemove = component;
                }
                
                if (ImGui::MenuItem(ICON_FK_CLONE " Copy Component"))
                {
                    // Clone via serialize/deserialize through componentFactory
                    Component* copy = enjinn::createComponent(component->getComponentName());
                    if (copy)
                    {
                        sfs::SafeSafeKeyValueData tempData;
                        component->serialize(tempData);
                        copy->deserialize(tempData);
                        copy->node = node;
                        node->components.push_back(copy);
                        copy->onCreate();
                    }
                }
                
                if (ImGui::MenuItem(ICON_FK_ARROW_UP " Move Up", nullptr, false, i > 0))
                {
                    if (i > 0)
                    {
                        std::swap(node->components[i], node->components[i-1]);
                    }
                }
                
                if (ImGui::MenuItem(ICON_FK_ARROW_DOWN " Move Down", nullptr, false, i < node->components.size()-1))
                {
                    if (i < node->components.size()-1)
                    {
                        std::swap(node->components[i], node->components[i+1]);
                    }
                }
                
                ImGui::EndPopup();
            }
            
            if (componentOpen)
            {
                ImGui::Indent(8.0f);
                ImGui::Spacing();
                
                // Call component's custom inspector
                component->onInspector();
                
                ImGui::Spacing();
                ImGui::Unindent(8.0f);
            }
            
            ImGui::PopID();
            ImGui::Spacing();
        }
    }
    
    void InspectorWindow::drawAddComponentMenu(Node* node)
    {
        ImGui::Separator();
        ImGui::Spacing();
        
        // Centered Add Component button
        float buttonWidth = ImGui::GetContentRegionAvail().x;
        if(ImGui::Button(ICON_FK_PLUS " Add Component", ImVec2(buttonWidth, 30)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if(ImGui::BeginPopup("AddComponentPopup"))
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Select a component to add:");
            ImGui::Separator();
            
            // Rendering
            if (ImGui::BeginMenu(ICON_FK_PICTURE_O " Rendering"))
            {
                if(ImGui::MenuItem(ICON_FK_PICTURE_O " Sprite Component"))
                {
                    node->addComponent<GenericSpriteComponent>();
                }
                
                if(ImGui::MenuItem(ICON_FK_CUBE " Mesh Renderer"))
                {
                    node->addComponent<enjinn::MeshRendererComponent>();
                }
                
                if(ImGui::MenuItem(ICON_FK_PAINT_BRUSH " Material"))
                {
                    node->addComponent<enjinn::MaterialComponent>();
                }
                
                ImGui::EndMenu();
            }
            
            // Physics
            if (ImGui::BeginMenu(ICON_FK_CIRCLE_O " Physics"))
            {
                if(ImGui::MenuItem(ICON_FK_CIRCLE_O " Physics Body"))
                {
                    node->addComponent<GenericPhysicsComponent>();
                }
                
                if(ImGui::MenuItem(ICON_FK_SQUARE_O " Box Collider"))
                {
                    auto* phys = node->addComponent<GenericPhysicsComponent>();
                    phys->size = {1.0f, 1.0f};
                }
                
                if(ImGui::MenuItem(ICON_FK_CIRCLE_O " Circle Collider"))
                {
                    auto* phys = node->addComponent<GenericPhysicsComponent>();
                    phys->size = {0.5f, 0.5f};
                }
                
                ImGui::EndMenu();
            }
            
            // Audio
            if (ImGui::BeginMenu(ICON_FK_VOLUME_UP " Audio"))
            {
                if(ImGui::MenuItem(ICON_FK_VOLUME_UP " Audio Source"))
                {
                    node->addComponent<enjinn::AudioSource>();
                }
                
                if(ImGui::MenuItem(ICON_FK_HEADPHONES " Audio Listener"))
                {
                    node->addComponent<enjinn::AudioListener>();
                }
                
                ImGui::EndMenu();
            }
            
            // Lighting
            if (ImGui::BeginMenu(ICON_FK_LIGHTBULB_O " Lighting"))
            {
                if(ImGui::MenuItem(ICON_FK_LIGHTBULB_O " Point Light"))
                {
                    auto* light = node->addComponent<enjinn::LightComponent>();
                    light->lightType = enjinn::LightType::Point;
                }
                
                if(ImGui::MenuItem(ICON_FK_SUN_O " Directional Light"))
                {
                    auto* light = node->addComponent<enjinn::LightComponent>();
                    light->lightType = enjinn::LightType::Directional;
                }
                
                if(ImGui::MenuItem(ICON_FK_BOLT " Spot Light"))
                {
                    auto* light = node->addComponent<enjinn::LightComponent>();
                    light->lightType = enjinn::LightType::Spot;
                }
                
                ImGui::EndMenu();
            }
            
            // Camera
            if(ImGui::MenuItem(ICON_FK_VIDEO_CAMERA " Camera"))
            {
                node->addComponent<enjinn::CameraComponent>();
            }
            
            // Scripts
            if (ImGui::BeginMenu(ICON_FK_FILE_CODE_O " Scripts"))
            {
                if(ImGui::MenuItem(ICON_FK_FILE_CODE_O " Custom Script"))
                {
                    node->addComponent<enjinn::ScriptComponent>();
                }
                
                ImGui::EndMenu();
            }
            
            ImGui::EndPopup();
        }
    }
}
