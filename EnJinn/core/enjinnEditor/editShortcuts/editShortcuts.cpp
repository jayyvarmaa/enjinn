

#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include "editShortcuts.h"
#include <imgui.h>
#include <enjinnImgui/enjinnImgui.h>
#include <safeSave/safeSave.h>
#include <filesystem>

static constexpr const char* SHORTCUTS_SAVE_PATH = "engineResources/engineSaves/shortcuts.dat";

void enjinn::EditShortcutsWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &imguiIdManager)
{
	imguiId = imguiIdManager.getImguiIds(2);
}

void enjinn::EditShortcutsWindow::update(enjinn::ShortcutManager &shortcutManager, bool &open)
{


	ImGui::SetNextWindowSize({400, 500});

	ImGui::PushID(imguiId);

	if (ImGui::Begin(ICON_NAME, &open,
		ImGuiWindowFlags_NoDocking | 
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
		))
	{
		
		ImGui::Text("Edit shortcuts\n");

		if (ImGui::BeginChild(imguiId + 1, {}, true))
		{


			ImGui::Columns(2, 0, false);

			for (auto &shortcut : shortcutManager.registeredShortcuts)
			{

				ImGui::Text(shortcut.first.c_str());

				ImGui::NextColumn();


				char input[256] = {};
				std::strncpy(input, shortcut.second.shortcut.c_str(), sizeof(input));
				
				int flags = ImGuiInputTextFlags_EnterReturnsTrue;
				if (!shortcut.second.editable)
				{
					flags = flags | ImGuiInputTextFlags_ReadOnly;
				}

				if (
					ImGui::InputText(("##" + shortcut.first).c_str(),
					input, sizeof(input), flags)
					)
				{
					shortcut.second.shortcut = enjinn::normalizeShortcutName(input);

				}

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			ImGui::EndChild();
		}

		ImGui::Separator();
		if (ImGui::Button(ICON_FK_FLOPPY_O " Save Shortcuts"))
		{
			sfs::SafeSafeKeyValueData data = {};
			int idx = 0;
			for (auto& [name, sc] : shortcutManager.registeredShortcuts)
			{
				std::string key = "sc" + std::to_string(idx);
				data.setString((key + ".name"), name);
				data.setString((key + ".shortcut"), sc.shortcut);
				idx++;
			}
			data.setInt("count", idx);
			sfs::safeSave(data, SHORTCUTS_SAVE_PATH, false);
		}
		
		ImGui::SameLine();
		if (ImGui::Button(ICON_FK_FOLDER_OPEN " Load Shortcuts"))
		{
			sfs::SafeSafeKeyValueData data = {};
			if (sfs::safeLoad(data, SHORTCUTS_SAVE_PATH, false) == sfs::noError)
			{
				int count = 0;
				data.getInt("count", count);
				for (int i = 0; i < count; i++)
				{
					std::string key = "sc" + std::to_string(i);
					std::string name;
					std::string shortcutStr;
					data.getString((key + ".name"), name);
					data.getString((key + ".shortcut"), shortcutStr);
					
					auto it = shortcutManager.registeredShortcuts.find(name);
					if (it != shortcutManager.registeredShortcuts.end() && it->second.editable)
					{
						it->second.shortcut = shortcutStr;
					}
				}
			}
		}

	}
	ImGui::End();

	ImGui::PopID();

}

#endif