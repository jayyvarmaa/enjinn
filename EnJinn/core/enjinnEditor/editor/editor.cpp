//////////////////////////////////////////
//editor.cpp
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////

#include "enjinnConfig.h"
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include "editor.h"
#include <iostream>
#include "IconsFontAwesome6.h"
#include "shortcutApi/shortcutApi.h"
#include <editShortcuts/editShortcuts.h>
#include <safeSave/safeSave.h>
#include <filesystem>
#include <string>
#include <phaseManager/phaseManager.h>
#include "imgui_internal.h"



#define DOCK_MAIN_WINDOW_SHORTCUT ICON_FK_EYE_SLASH " Hide main window"
#define LOGS_SHORTCUT ICON_FK_COMMENT_O " Logs window"
#define EDIT_SHORTCUTS ICON_FK_PENCIL_SQUARE " Edit shortcuts window"
#define CONTAINERS_SHORTCUTS ICON_FK_MICROCHIP " Containers window"
#define RELOAD_DLL_SHORTCUTS ICON_FK_REFRESH " Reload dll"
#define TRANSPARENT_EDITOR_WINDOW ICON_FK_EYE " Transparent Editor window"
#define CONSOLE_WINDOW ICON_FK_TERMINAL " Console window"
#define ASSET_MANAGER_WINDOW ICON_FK_FILES_O " Asset manager"
#define OPENGL_ERRORS_WINDOW ICON_FK_EXCLAMATION_TRIANGLE " opengl errors"
#define HIERARCHY_WINDOW ICON_FK_LIST " Hierarchy"
#define INSPECTOR_WINDOW ICON_FK_INFO " Inspector"
#define SCENE_VIEW_WINDOW ICON_FK_PICTURE_O " Scene View"


void enjinn::Editor::init(enjinn::ShortcutManager &shortcutManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDManager)
{

	shortcutManager.registerShortcut(DOCK_MAIN_WINDOW_SHORTCUT, "Ctrl+Alt+D", &optionsFlags.hideMainWindow);
	shortcutManager.registerShortcut(LOGS_SHORTCUT, "Ctrl+L", &windowFlags.logsWindow);
	shortcutManager.registerShortcut(EDIT_SHORTCUTS, "", &windowFlags.editShortcutsWindow);
	shortcutManager.registerShortcut(CONTAINERS_SHORTCUTS, "Ctrl+M", &windowFlags.containerManager);
	shortcutManager.registerShortcut(RELOAD_DLL_SHORTCUTS, "Ctrl+Alt+R", &shouldReloadDll);
	shortcutManager.registerShortcut(TRANSPARENT_EDITOR_WINDOW, "Ctrl+Alt+T", &windowFlags.transparentWindow);
	shortcutManager.registerShortcut(CONSOLE_WINDOW, "Ctrl+C", &windowFlags.consoleWindow);
	shortcutManager.registerShortcut(ASSET_MANAGER_WINDOW, "Ctrl+Alt+A", &windowFlags.assetManagerWindow);
	shortcutManager.registerShortcut(OPENGL_ERRORS_WINDOW, "Ctrl+Alt+O", &windowFlags.openglErrorsWindow);
    shortcutManager.registerShortcut(HIERARCHY_WINDOW, "", &windowFlags.hierarchyWindow);
    shortcutManager.registerShortcut(INSPECTOR_WINDOW, "", &windowFlags.inspectorWindow);
    shortcutManager.registerShortcut(SCENE_VIEW_WINDOW, "", &windowFlags.sceneViewWindow);

	imguiId = imguiIDManager.getImguiIds(1);

	logWindow.init(imguiIDManager);
	editShortcutsWindow.init(imguiIDManager);
	containersWindow.init(imguiIDManager);
	consoleWindow.init(imguiIDManager);
	assetManagerWindow.init(imguiIDManager);
	openglLogsWindow.init(imguiIDManager);
    hierarchyWindow.init(imguiIDManager);
    inspectorWindow.init(imguiIDManager);
    sceneViewWindow.init(imguiIDManager);

	if (sfs::safeLoad(&optionsFlags, sizeof(optionsFlags), ENJINN_ENGINE_SAVES_PATH "options", false) != sfs::noError)
	{
		optionsFlags = {};
	}

	if (sfs::safeLoad(&windowFlags, sizeof(windowFlags), ENJINN_ENGINE_SAVES_PATH "window", false) != sfs::noError)
	{
		windowFlags = {};
	}
	
	std::string path = ENJINN_ENGINE_RESOURCES_PATH "LOGO.png";
	std::cout << "Loading logo from: " << path << "\n";
	if (std::filesystem::exists(path))
	{
		std::cout << "File exists.\n";
	}
	else
	{
		std::cout << "File DOES NOT exist.\n";
	}

	logoTexture.loadFromFile(path.c_str());

    assetManagerWindow.loadData();
}



void enjinn::Editor::update(const enjinn::Input &input,
	enjinn::ShortcutManager &shortcutManager, enjinn::LogManager &logs, 
	enjinn::PushNotificationManager &pushNotificationManager, enjinn::LoadedDll &loadedDll
	,enjinn::ContainerManager &containerManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager)
{

#pragma region push notification if hide window

	if (lastHideWindowState == 0 && optionsFlags.hideMainWindow)
	{
		std::string message = "Press ";
		message += shortcutManager.getShortcut(DOCK_MAIN_WINDOW_SHORTCUT);
		message += " to restore the main window.";

		pushNotificationManager.pushNotification(message.c_str());
	}

	lastHideWindowState = optionsFlags.hideMainWindow;
#pragma endregion


	if (!optionsFlags.hideMainWindow)
	{

#pragma region docking space init
		ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_MenuBar;
		//if (optionsFlags.hideMainWindow)
		{
			mainWindowFlags = ImGuiWindowFlags_MenuBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoTitleBar;

			ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
			ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;
			ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y), 0);
		}
#pragma endregion

#pragma region main editor window

		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.3f, 1.0f));
	
	
		ImGui::PushID(imguiId);

		ImGui::SetNextWindowBgAlpha(0);

		if (ImGui::Begin(
			"Main window",
			/*p_open=*/nullptr,
			mainWindowFlags
			)
			)
		{


			//if (optionsFlags.dockMainWindow)
			{
				static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
				ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
				ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}

		#pragma region menu
			if (ImGui::BeginMenuBar())
			{

				if (ImGui::BeginMenu(ICON_FK_COGS " EnJinn"))
				{
					//todo submit tasks to the engine (usefull for this and also in gameplay)

					if (ImGui::MenuItem(ICON_FK_REFRESH " Reload dll",
						shortcutManager.getShortcut(RELOAD_DLL_SHORTCUTS), nullptr))
					{
						shouldReloadDll = true;
					}


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Options"))
				{


					ImGui::MenuItem(DOCK_MAIN_WINDOW_SHORTCUT,
						shortcutManager.getShortcut(DOCK_MAIN_WINDOW_SHORTCUT), &optionsFlags.hideMainWindow);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu(ICON_FK_GAMEPAD " Gameplay"))
				{
					if (loadedDll.containerInfo.empty())
					{
						ImGui::TextDisabled("No games available");
					}
					else
					{
						for (auto &c : loadedDll.containerInfo)
						{
							if (ImGui::MenuItem(c.containerName.c_str()))
							{
								containerManager.createContainer(c.containerName, loadedDll, logs, imguiIDsManager, &consoleWindow, std::string());
							}
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu(ICON_FK_WINDOW_MAXIMIZE " Windows"))
				{
					auto& phaseMgr = enjinn::PhaseManager::Get();
					
					// PHASE 1: Logs only
					ImGui::MenuItem(enjinn::LogWindow::ICON_NAME,
						shortcutManager.getShortcut(LOGS_SHORTCUT), &windowFlags.logsWindow);
					
					// PHASE 2: Core Systems
					if (phaseMgr.IsFeatureUnlocked(enjinn::DevelopmentPhase::Phase2_CoreSystems))
					{
						ImGui::MenuItem(enjinn::ContainersWindow::ICON_NAME,
							shortcutManager.getShortcut(CONTAINERS_SHORTCUTS), &windowFlags.containerManager);
						
						ImGui::MenuItem(enjinn::AssetManagerWindow::ICON_NAME,
							shortcutManager.getShortcut(ASSET_MANAGER_WINDOW), &windowFlags.assetManagerWindow);
					}
					
					// PHASE 3: Editor Suite
					if (phaseMgr.IsFeatureUnlocked(enjinn::DevelopmentPhase::Phase3_Editor))
					{
						ImGui::MenuItem(enjinn::ConsoleWindow::ICON_NAME,
							shortcutManager.getShortcut(CONSOLE_WINDOW), &windowFlags.consoleWindow);
						
						ImGui::MenuItem(enjinn::HierarchyWindow::ICON_NAME,
							shortcutManager.getShortcut(HIERARCHY_WINDOW), &windowFlags.hierarchyWindow);
						
						ImGui::MenuItem(enjinn::InspectorWindow::ICON_NAME,
							shortcutManager.getShortcut(INSPECTOR_WINDOW), &windowFlags.inspectorWindow);
						
						ImGui::MenuItem(enjinn::SceneViewWindow::ICON_NAME,
							shortcutManager.getShortcut(SCENE_VIEW_WINDOW), &windowFlags.sceneViewWindow);
					}
					
					// PHASE 4: Physics & Gameplay
					if (phaseMgr.IsFeatureUnlocked(enjinn::DevelopmentPhase::Phase4_PhysicsGameplay))
					{
						ImGui::MenuItem(enjinn::OpenglLogsWindow::ICON_NAME,
							shortcutManager.getShortcut(OPENGL_ERRORS_WINDOW), &windowFlags.openglErrorsWindow);
					}

					ImGui::EndMenu();

				}

				if (ImGui::BeginMenu(ICON_FK_COG " Settings"))
				{
					ImGui::MenuItem(enjinn::EditShortcutsWindow::ICON_NAME,
						shortcutManager.getShortcut(EDIT_SHORTCUTS), &windowFlags.editShortcutsWindow);

					enjinn::enjinnImgui::displayMemorySizeToggle();

					ImGui::MenuItem(TRANSPARENT_EDITOR_WINDOW,
						shortcutManager.getShortcut(TRANSPARENT_EDITOR_WINDOW), 
						&windowFlags.transparentWindow);

					ImGui::Separator();

					// SECRET: Development Phase Control
					auto& phaseMgr = enjinn::PhaseManager::Get();

					if (phaseMgr.IsPhaseSelectorUnlocked())
					{
						// Phase selector visible
						int currentPhase = static_cast<int>(phaseMgr.GetCurrentPhase());
						const char* phaseNames[] = {
							"Phase 1: Foundation",
							"Phase 2: Core Systems", 
							"Phase 3: Editor Suite",
							"Phase 4: Physics & Gameplay",
							"Phase 5: Complete"
						};
						
						if (ImGui::Combo("Development Phase", &currentPhase, phaseNames, 5))
						{
							phaseMgr.SetPhase(static_cast<enjinn::DevelopmentPhase>(currentPhase));
							logs.log("Phase changed");
						}
						
						ImGui::Separator();
					}
					else
					{
						// Hidden unlock field
						if (ImGui::InputTextWithHint("##secretcode", "...", secretCodeBuffer, sizeof(secretCodeBuffer), 
							ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_Password))
						{
							if (phaseMgr.TryUnlockPhaseSelector(secretCodeBuffer))
							{
								logs.log("Phase selector unlocked!");
								pushNotificationManager.pushNotification("Dev mode activated");
								memset(secretCodeBuffer, 0, sizeof(secretCodeBuffer));
							}
						}
					}

					if (ImGui::BeginMenu(ICON_FK_COLUMNS " Layouts"))
					{
						if (ImGui::BeginMenu(ICON_FK_FLOPPY_O " Save Layout"))
						{
							static char buf[64] = "myLayout";
							ImGui::InputText("Name", buf, sizeof(buf));
							if (ImGui::Button("Save"))
							{
								std::string path = ENJINN_ENGINE_SAVES_PATH "layouts/";
								std::filesystem::create_directories(path);
								path += buf;
								path += ".ini";
								ImGui::SaveIniSettingsToDisk(path.c_str());
								logs.log(("Layout saved to: " + path).c_str());
							}
							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu(ICON_FK_FOLDER_OPEN " Load Layout"))
						{
							std::string path = ENJINN_ENGINE_SAVES_PATH "layouts/";
							if(std::filesystem::exists(path))
							{
								for (const auto & entry : std::filesystem::directory_iterator(path))
								{
									std::string filename = entry.path().filename().string();
									if (ImGui::MenuItem(filename.c_str()))
									{
										ImGui::LoadIniSettingsFromDisk(entry.path().string().c_str());
										logs.log(("Loaded layout: " + filename).c_str());
									}
								}
							}
							else
							{
								ImGui::TextDisabled("No save folder found");
							}

							ImGui::EndMenu();
						}
						
						ImGui::Separator();

						if (ImGui::MenuItem(ICON_FK_REFRESH " Reset Defaults"))
						{
                            // Load custom layout requested by user
                            std::string customLayout = ENJINN_ENGINE_SAVES_PATH "layouts/new.ini";
                            std::string targetIni = "imgui.ini";
                            
                            bool resetSuccess = false;
                            
                            if (std::filesystem::exists(customLayout))
                            {
                                try {
                                    std::filesystem::copy_file(customLayout, targetIni, std::filesystem::copy_options::overwrite_existing);
                                    resetSuccess = true;
                                } catch(...) {
                                    logs.log("Failed to copy layout file", enjinn::logError);
                                }
                            }
                            
                            if(!resetSuccess)
                            {
                                // Fallback if file doesn't exist
                                if (std::filesystem::exists(targetIni))
                                {
                                    std::filesystem::remove(targetIni);
                                }
                                logs.log("Layout reset to factory simple default (custom layout not found)");
                            }
                            else
                            {
                                logs.log("Layout reset to 'new.ini' configuration");
                            }
                            
							ImGui::LoadIniSettingsFromDisk(targetIni.c_str()); 
						}
						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (logoTexture.id)
				{
					// Use GetFrameHeight to fit inside the menu bar
					float height = ImGui::GetFrameHeight(); 
					
					// Add some padding if needed
					float padding = 4.0f;
					float drawHeight = height - padding;
					
					if(drawHeight > 0)
					{
						float aspect = (float)logoTexture.GetSize().x / (float)logoTexture.GetSize().y;
						float width = drawHeight * aspect;

						// Center the image in the window (not just the remaining space)
						float windowWidth = ImGui::GetWindowWidth();
						
						// Ensure we don't overlap with existing menu items if the window is too small,
						// but primarily try to center it.
						float cursorX = (windowWidth - width) * 0.5f;
						
						// Only set cursor if it's past the current cursor position (avoid overwriting text)
						if (cursorX > ImGui::GetCursorPosX())
						{
							ImGui::SetCursorPosX(cursorX);
							// Center vertically in the bar
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 0.5f);
							// UVs flipped to (0,1) and (1,0) to correct vertical flip
							ImGui::Image((void*)(intptr_t)logoTexture.id, { width, drawHeight }, ImVec2(0, 1), ImVec2(1, 0));
						}
					}
				}

				ImGui::EndMenuBar();
			}
		#pragma endregion

		}
		ImGui::End();

		ImGui::PopID();

	#pragma endregion

	}

	if (windowFlags.transparentWindow)
	{
		ImGuiStyle &style = ::ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
	}
	else
	{
		ImGuiStyle &style = ::ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 1.f;
	}

#pragma region log window
	if (windowFlags.logsWindow)
	{
		logWindow.update(logs, windowFlags.logsWindow);
	}
#pragma endregion

#pragma region shortcuts window
	if (windowFlags.editShortcutsWindow)
	{
		editShortcutsWindow.update(shortcutManager, windowFlags.editShortcutsWindow);
	}
#pragma endregion

#pragma region containers window
	if (windowFlags.containerManager)
	{
		containersWindow.update(logs, windowFlags.containerManager, 
			loadedDll, containerManager, imguiIDsManager, &consoleWindow);
	}
#pragma endregion

#pragma region console
	if (windowFlags.consoleWindow)
	{
		consoleWindow.update(windowFlags.consoleWindow);
	}
#pragma endregion

#pragma region asset manager window

	if (windowFlags.assetManagerWindow)
	{
		assetManagerWindow.update(windowFlags.assetManagerWindow, containerManager, loadedDll, 
			logs, imguiIDsManager, &consoleWindow);
	}

#pragma endregion

#pragma region opengl logs

	if (windowFlags.openglErrorsWindow)
	{
		openglLogsWindow.update(windowFlags.openglErrorsWindow);
	}

#pragma endregion

#pragma region Scene Graph Editor
    // Check if we have an active container
    if(!containerManager.runningContainers.empty())
    {
        // Find focused container
        enjinn::RuntimeContainer* activeContainer = &containerManager.runningContainers.begin()->second; // Default to first
        
        for(auto& pair : containerManager.runningContainers)
        {
            // Check if this container was focused last frame
            // Using lastFrameFocus from RuntimeContainer
            if(pair.second.lastFrameFocus) 
            {
                activeContainer = &pair.second;
                break;
            }
        }

        if(activeContainer && activeContainer->pointer)
        {
            enjinn::Scene* scene = activeContainer->pointer->getScene();
            
            // Enforce flags
            windowFlags.hierarchyWindow = true;
            windowFlags.inspectorWindow = true;
            windowFlags.sceneViewWindow = true;

            // Hierarchy
            hierarchyWindow.update(scene, windowFlags.hierarchyWindow);
            
            // Inspector
            inspectorWindow.update(hierarchyWindow.selectedNode, windowFlags.inspectorWindow);

            // Scene View
            unsigned int textureId = activeContainer->requestedContainerInfo.requestedFBO.texture;
            sceneViewWindow.update(textureId, windowFlags.sceneViewWindow);
        }
    }
#pragma endregion


}

void enjinn::Editor::saveFlagsData()
{

	sfs::safeSave(&optionsFlags, sizeof(optionsFlags), ENJINN_ENGINE_SAVES_PATH "options", false);
	sfs::safeSave(&windowFlags, sizeof(windowFlags), ENJINN_ENGINE_SAVES_PATH "window", false);

    assetManagerWindow.saveData();


}

#endif

