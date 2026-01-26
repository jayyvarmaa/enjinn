



#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR


#include "logWindow.h"
#include <imgui.h>
#include <IconsFontAwesome6.h>
#include <enjinnImgui/enjinnImgui.h>



void enjinn::LogWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
{
	imguiId = idManager.getImguiIds();
}

void enjinn::LogWindow::update(enjinn::LogManager &logManager, bool &open)
{
	ImGui::PushID(imguiId);

	// Set default window size for first use
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin(ICON_NAME, &open))
	{
		ImGui::End();
		ImGui::PopID();
		return;
	}

	
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &autoScroll);
		ImGui::EndPopup();
	}

	
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
		{logManager.internalLogs.clear();}
	ImGui::SameLine();
	
	if(ImGui::Button("Copy"))
		{ImGui::LogToClipboard();}
	ImGui::SameLine();

	filter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	if (filter.IsActive())
	{
		for (auto i : logManager.internalLogs)
		{
			if (filter.PassFilter(i.c_str()))
			{
				ImGui::TextUnformatted(i.c_str());
			}
		}

	}
	else
	{
		for (auto i : logManager.internalLogs)
		{
			ImGui::TextUnformatted(i.c_str());
		}
	}
	ImGui::PopStyleVar();

	if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();

	ImGui::PopID();
}

#endif

