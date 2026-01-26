#include "sceneViewWindow.h"
#include <imgui.h>

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
            ImVec2 size = ImGui::GetContentRegionAvail();
            if(textureId)
            {
                ImGui::Image((void*)(intptr_t)textureId, size, ImVec2(0, 1), ImVec2(1, 0));
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopID();
    }
}
