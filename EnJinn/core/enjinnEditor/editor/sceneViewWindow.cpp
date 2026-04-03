#include "sceneViewWindow.h"
#include <imgui.h>

namespace
{
    ImVec2 projectToSceneView(const glm::vec3& worldPos, const glm::mat4& viewMatrix,
        const glm::mat4& projectionMatrix, const ImVec2& contentMin, const ImVec2& size)
    {
        const glm::vec4 clip = projectionMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);
        if (clip.w <= 0.0f)
        {
            return ImVec2(-1000.0f, -1000.0f);
        }

        const glm::vec3 ndc = glm::vec3(clip) / clip.w;
        return ImVec2(
            contentMin.x + ((ndc.x + 1.0f) * 0.5f) * size.x,
            contentMin.y + ((1.0f - ndc.y) * 0.5f) * size.y);
    }
}

namespace enjinn
{
    void SceneViewWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
    {
        windowId = idManager.getImguiIds(1);
    }

    void SceneViewWindow::update(unsigned int textureId, bool &open)
    {
        // Enforce visibility
        ImGui::PushID(windowId);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        if(ImGui::Begin(ICON_NAME, nullptr)) // Pass nullptr to remove close button
        {
            ImGui::SetCursorPos(ImVec2(8.0f, 8.0f));
            if (ImGui::Button("Translate")) { gizmo.setMode(GizmoMode::Translate); }
            ImGui::SameLine();
            if (ImGui::Button("Rotate")) { gizmo.setMode(GizmoMode::Rotate); }
            ImGui::SameLine();
            if (ImGui::Button("Scale")) { gizmo.setMode(GizmoMode::Scale); }
            ImGui::SameLine();
            ImGui::TextUnformatted("Shortcuts: W / E / R");

            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
            {
                if (ImGui::IsKeyPressed(ImGuiKey_W)) { gizmo.setMode(GizmoMode::Translate); }
                if (ImGui::IsKeyPressed(ImGuiKey_E)) { gizmo.setMode(GizmoMode::Rotate); }
                if (ImGui::IsKeyPressed(ImGuiKey_R)) { gizmo.setMode(GizmoMode::Scale); }
            }

            ImGui::SetCursorPosY(36.0f);
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 contentMin = ImGui::GetCursorScreenPos();
            
            // Render scene texture
            if(textureId)
            {
                ImGui::Image((void*)(intptr_t)textureId, size, ImVec2(0, 1), ImVec2(1, 0));
            }
            
            // Handle gizmo interaction
            if (selectedNode && gizmo.getMode() != GizmoMode::None)
            {
                // Get mouse input
                ImGuiIO& io = ImGui::GetIO();
                glm::vec2 mousePos = glm::vec2(io.MousePos.x, io.MousePos.y);
                glm::vec2 mouseDelta = mousePos - lastMousePos;
                lastMousePos = mousePos;
                
                bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
                bool mouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
                
                // Check if mouse is over scene view
                bool isHovered = ImGui::IsWindowHovered();
                
                if (isHovered)
                {
                    // Update gizmo render data
                    GizmoRenderData renderData;
                    renderData.position = selectedNode->localPosition;
                    renderData.rotation = selectedNode->localRotation;
                    renderData.scale = selectedNode->localScale;
                    renderData.viewMatrix = viewMatrix;
                    renderData.projectionMatrix = projectionMatrix;
                    
                    // Update gizmo (returns true if actively manipulating)
                    gizmoActive = gizmo.update(selectedNode, renderData, mousePos, mouseDelta, 
                                              mouseDown, mouseClicked);
                    
                    // Render gizmo overlay
                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    ImVec2 gizmoCenter = projectToSceneView(selectedNode->localPosition,
                        viewMatrix, projectionMatrix, contentMin, size);
                    if (gizmoCenter.x < contentMin.x - 500.0f)
                    {
                        gizmoActive = false;
                        ImGui::End();
                        ImGui::PopStyleVar();
                        ImGui::PopID();
                        return;
                    }
                    
                    // Draw gizmo visualization here
                    if (gizmo.getMode() == GizmoMode::Translate)
                    {
                        float arrowLength = 60.0f;
                        
                        // X axis - Red
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x + arrowLength, gizmoCenter.y),
                                        IM_COL32(255, 0, 0, 255), 3.0f);
                        
                        // Y axis - Green
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x, gizmoCenter.y - arrowLength),
                                        IM_COL32(0, 255, 0, 255), 3.0f);
                        
                        // Z axis - Blue (diagonal for 2D representation)
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x - arrowLength * 0.7f, gizmoCenter.y + arrowLength * 0.7f),
                                        IM_COL32(0, 0, 255, 255), 3.0f);
                    }
                    else if (gizmo.getMode() == GizmoMode::Rotate)
                    {
                        float radius = 50.0f;
                        
                        drawList->AddCircle(gizmoCenter, radius, IM_COL32(255, 0, 0, 200), 32, 2.0f);
                        drawList->AddCircle(gizmoCenter, radius * 1.2f, IM_COL32(0, 255, 0, 200), 32, 2.0f);
                        drawList->AddCircle(gizmoCenter, radius * 1.4f, IM_COL32(0, 0, 255, 200), 32, 2.0f);
                    }
                    else if (gizmo.getMode() == GizmoMode::Scale)
                    {
                        float boxSize = 10.0f;
                        float lineLength = 50.0f;
                        
                        // X axis
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x + lineLength, gizmoCenter.y),
                                        IM_COL32(255, 0, 0, 255), 2.0f);
                        drawList->AddRectFilled(ImVec2(gizmoCenter.x + lineLength - boxSize/2, gizmoCenter.y - boxSize/2),
                                              ImVec2(gizmoCenter.x + lineLength + boxSize/2, gizmoCenter.y + boxSize/2),
                                              IM_COL32(255, 0, 0, 255));
                        
                        // Y axis
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x, gizmoCenter.y - lineLength),
                                        IM_COL32(0, 255, 0, 255), 2.0f);
                        drawList->AddRectFilled(ImVec2(gizmoCenter.x - boxSize/2, gizmoCenter.y - lineLength - boxSize/2),
                                              ImVec2(gizmoCenter.x + boxSize/2, gizmoCenter.y - lineLength + boxSize/2),
                                              IM_COL32(0, 255, 0, 255));
                        
                        // Z axis
                        drawList->AddLine(gizmoCenter, 
                                        ImVec2(gizmoCenter.x - lineLength * 0.7f, gizmoCenter.y + lineLength * 0.7f),
                                        IM_COL32(0, 0, 255, 255), 2.0f);
                        drawList->AddRectFilled(ImVec2(gizmoCenter.x - lineLength * 0.7f - boxSize/2, 
                                                      gizmoCenter.y + lineLength * 0.7f - boxSize/2),
                                              ImVec2(gizmoCenter.x - lineLength * 0.7f + boxSize/2,
                                                      gizmoCenter.y + lineLength * 0.7f + boxSize/2),
                                              IM_COL32(0, 0, 255, 255));
                    }
                }
                else
                {
                    gizmoActive = false;
                }
                
                wasMouseDown = mouseDown;
            }
            else
            {
                gizmoActive = false;
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopID();
    }
}
