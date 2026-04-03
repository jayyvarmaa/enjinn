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
#include "../../sharedRuntime/windowSystemm/window.h" // Native Window Flag
#include <safeSave/safeSave.h>
#include <GLFW/glfw3.h> // Custom Title Bar Controls
#include <filesystem>
#include <string>
#include <algorithm>
#include <phaseManager/phaseManager.h>
#include <svgTexture/svgTexture.h>
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
#define PROFILER_WINDOW ICON_FK_TACHOMETER " Profiler"
#define PREFAB_WINDOW ICON_FK_CUBES " Prefabs"
#define UNDO_HISTORY_WINDOW ICON_FK_HISTORY " Undo History"
#define MATERIAL_EDITOR_WINDOW ICON_FK_PAINT_BRUSH " Material Editor"
#define PROJECT_SETTINGS_WINDOW ICON_FK_SLIDERS " Project Settings"
#define BUILD_WINDOW ICON_FK_COG " Build"
#define DIAGNOSTIC_WINDOW ICON_FK_STETHOSCOPE " Diagnostics"


void enjinn::Editor::init(enjinn::ShortcutManager &shortcutManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDManager)
{

	// Restrict window dragging to explicit title bars only (avoid dragging from toolbar/menu area)
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;


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
	shortcutManager.registerShortcut(PROFILER_WINDOW, "Ctrl+Alt+P", &windowFlags.profilerWindow);
	shortcutManager.registerShortcut(PREFAB_WINDOW, "", &windowFlags.prefabWindow);
	shortcutManager.registerShortcut(UNDO_HISTORY_WINDOW, "", &windowFlags.undoHistoryWindow);
	shortcutManager.registerShortcut(MATERIAL_EDITOR_WINDOW, "", &windowFlags.materialEditorWindow);
	shortcutManager.registerShortcut(PROJECT_SETTINGS_WINDOW, "", &windowFlags.projectSettingsWindow);
	shortcutManager.registerShortcut(BUILD_WINDOW, "Ctrl+B", &windowFlags.buildWindow);
	shortcutManager.registerShortcut(DIAGNOSTIC_WINDOW, "Ctrl+Shift+D", &windowFlags.diagnosticWindow);
	logWindow.init(imguiIDManager);
	editShortcutsWindow.init(imguiIDManager);
	containersWindow.init(imguiIDManager);
	consoleWindow.init(imguiIDManager);
	assetManagerWindow.init(imguiIDManager);
	openglLogsWindow.init(imguiIDManager);
    hierarchyWindow.init(imguiIDManager);
    inspectorWindow.init(imguiIDManager);
    sceneViewWindow.init(imguiIDManager);
    profilerWindow.init(imguiIDManager);
    prefabWindow.init(imguiIDManager);
    undoHistoryWindow.init(imguiIDManager);
    materialEditorWindow.init(imguiIDManager);
    projectSettingsWindow.init(imguiIDManager);
    buildWindow.init(imguiIDManager);
    diagnosticWindow.init(imguiIDManager);

	if (sfs::safeLoad(&optionsFlags, sizeof(optionsFlags), ENJINN_ENGINE_SAVES_PATH "options", false) != sfs::noError)
	{
		optionsFlags = {};
	}

	if (sfs::safeLoad(&windowFlags, sizeof(windowFlags), ENJINN_ENGINE_SAVES_PATH "window", false) != sfs::noError)
	{
		windowFlags = {};
	}
	
	// Load SVG logo at high resolution for crisp display
	std::string svgPath = ENJINN_ENGINE_RESOURCES_PATH "LOGO.svg";
	std::cout << "Loading logo from: " << svgPath << "\n";
	if (std::filesystem::exists(svgPath))
	{
		std::cout << "SVG file exists.\n";
		// Rasterize at 1000px height for High Quality Supersampling (Scaled down in UI)
		logoTextureId = enjinn::loadSVGTexture(svgPath, 1000, &logoWidth, &logoHeight);
	}
	else
	{
		std::cout << "SVG file DOES NOT exist, falling back to PNG.\n";
		// Fallback to PNG if SVG not available
		std::string pngPath = ENJINN_ENGINE_RESOURCES_PATH "LOGO.png";
		if (std::filesystem::exists(pngPath))
		{
			gl2d::Texture fallbackTexture;
			fallbackTexture.loadFromFile(pngPath.c_str());
			logoTextureId = fallbackTexture.id;
			logoWidth = fallbackTexture.GetSize().x;
			logoHeight = fallbackTexture.GetSize().y;
		}
	}

    assetManagerWindow.loadData();
}



void enjinn::Editor::update(const enjinn::Input &input,
	enjinn::ShortcutManager &shortcutManager, enjinn::LogManager &logs, 
	enjinn::PushNotificationManager &pushNotificationManager, enjinn::LoadedDll &loadedDll
	,enjinn::ContainerManager &containerManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager)
{

#pragma region push notification if hide window
	static bool lastHideWindowState = false;

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
                
                // --- CUSTOM WINDOW BORDER ---
                // "Way to change thickness": Modify this variable.
				static float windowBorderThickness = 1.0f; 
                
                if (windowBorderThickness > 0.0f)
                {
                    ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImDrawList* drawList = ImGui::GetForegroundDrawList();
					ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
                    
                    // Positions (Inset by half thickness to stay inside)
                    float t = windowBorderThickness;
                    float half = t * 0.5f;
                    ImVec2 min = viewport->Pos;
                    ImVec2 max = ImVec2(min.x + viewport->Size.x, min.y + viewport->Size.y);
                    
                    // Left
                    drawList->AddLine(ImVec2(min.x + half, min.y), ImVec2(min.x + half, max.y), borderColor, t);
                    // Right
                    drawList->AddLine(ImVec2(max.x - half, min.y), ImVec2(max.x - half, max.y), borderColor, t);
                    // Bottom
                    drawList->AddLine(ImVec2(min.x, max.y - half), ImVec2(max.x, max.y - half), borderColor, t);
                    
                    // Resize Grip (Bottom Right Visual Hint)
                    // Like a modern "Bootstrap" resize handle equivalent
                    if (!glfwGetWindowAttrib(glfwGetCurrentContext(), GLFW_MAXIMIZED))
                    {
                        ImVec2 gripP1 = ImVec2(max.x - half, max.y - 12);
                        ImVec2 gripP2 = ImVec2(max.x - 12, max.y - half);
                        drawList->AddLine(gripP1, ImVec2(max.x - half, max.y - half), borderColor, t);
                        drawList->AddLine(ImVec2(max.x - half, max.y - half), gripP2, borderColor, t);
                    }
                }
                // -----------------------------
			}

		#pragma region menu
			// FramePadding Y determines the bar height: barH = FontSize + FramePadding.y * 2.
			// Keeping it pushed through EndMenuBar makes every BeginMenu hit-box fill the full bar.
			ImVec4 topBarColor = ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive];
			ImGui::PushStyleColor(ImGuiCol_MenuBarBg, topBarColor);

			// Match VS Code-like title bar height and spacing
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 3.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 3.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(6.0f, 3.0f));
			if (ImGui::BeginMenuBar())
			{
				// NOTE: do NOT PopStyleVar here – keep padding so item hot-areas span full bar height.

				ImGui::Dummy(ImVec2(10.0f, 0));

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

				ImGui::SameLine(0, 12.0f);

				if (ImGui::BeginMenu("Options"))
				{


					ImGui::MenuItem(DOCK_MAIN_WINDOW_SHORTCUT,
						shortcutManager.getShortcut(DOCK_MAIN_WINDOW_SHORTCUT), &optionsFlags.hideMainWindow);

					ImGui::EndMenu();
				}

				ImGui::SameLine(0, 12.0f);

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
							if (c.containerName.find("DiagnosticsLab") != std::string::npos &&
								c.containerName != "AIBehaviorDiagnosticsLab") { continue; }

							const char* menuLabel = c.containerName.c_str();
							if (c.containerName == "AIBehaviorDiagnosticsLab")
							{
								menuLabel = "AI & Behavior Lab";
							}

							if (ImGui::MenuItem(menuLabel))
							{
								containerManager.createContainer(c.containerName, loadedDll, logs, imguiIDsManager, &consoleWindow, std::string());
							}
						}
					}
					ImGui::EndMenu();
				}

				ImGui::SameLine(0, 12.0f);

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
						
						ImGui::MenuItem(enjinn::PrefabWindow::ICON_NAME,
							shortcutManager.getShortcut(PREFAB_WINDOW), &windowFlags.prefabWindow);
						
						ImGui::MenuItem(enjinn::MaterialEditorWindow::ICON_NAME,
							shortcutManager.getShortcut(MATERIAL_EDITOR_WINDOW), &windowFlags.materialEditorWindow);
						
						ImGui::MenuItem(enjinn::ProjectSettingsWindow::ICON_NAME,
							shortcutManager.getShortcut(PROJECT_SETTINGS_WINDOW), &windowFlags.projectSettingsWindow);
						
						ImGui::MenuItem(enjinn::BuildWindow::ICON_NAME,
							shortcutManager.getShortcut(BUILD_WINDOW), &windowFlags.buildWindow);
					
						ImGui::MenuItem(enjinn::UndoHistoryWindow::ICON_NAME,
							shortcutManager.getShortcut(UNDO_HISTORY_WINDOW), &windowFlags.undoHistoryWindow);
					}
					
					// PHASE 4: Physics & Gameplay
					if (phaseMgr.IsFeatureUnlocked(enjinn::DevelopmentPhase::Phase4_PhysicsGameplay))
					{
						ImGui::MenuItem(enjinn::OpenglLogsWindow::ICON_NAME,
							shortcutManager.getShortcut(OPENGL_ERRORS_WINDOW), &windowFlags.openglErrorsWindow);
						
						ImGui::MenuItem(enjinn::ProfilerWindow::ICON_NAME,
							shortcutManager.getShortcut(PROFILER_WINDOW), &windowFlags.profilerWindow);
					}

					ImGui::EndMenu();

				}

				ImGui::SameLine(0, 12.0f);

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

				// Native window chrome is now used; skip drawing custom logo and window buttons.

				ImGui::EndMenuBar();
			}
			ImGui::PopStyleVar(3); // FramePadding + ItemSpacing + ItemInnerSpacing for menu bar
			ImGui::PopStyleColor();
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
			sceneViewWindow.setSelectedNode(hierarchyWindow.selectedNode);

			glm::mat4 viewMatrix(1.0f);
			glm::mat4 projectionMatrix(1.0f);
			if (activeContainer->pointer->getEditorCameraMatrices(viewMatrix, projectionMatrix))
			{
				sceneViewWindow.setCameraMatrices(viewMatrix, projectionMatrix);
			}
            
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
            
            // Prefab Window
            if (windowFlags.prefabWindow)
            {
                prefabWindow.setScene(scene);
                prefabWindow.setSelectedNode(hierarchyWindow.selectedNode);
                prefabWindow.update(windowFlags.prefabWindow);
            }
            
            // Undo History Window
            if (windowFlags.undoHistoryWindow)
            {
                undoHistoryWindow.update(undoRedoManager, windowFlags.undoHistoryWindow);
            }
        }
    }
#pragma endregion

#pragma region standalone windows
    // Profiler Window
    if (windowFlags.profilerWindow)
    {
        profilerWindow.update(windowFlags.profilerWindow);
    }
    
    // Material Editor Window
    if (windowFlags.materialEditorWindow)
    {
        materialEditorWindow.update(windowFlags.materialEditorWindow);
    }
    
    // Project Settings Window
    if (windowFlags.projectSettingsWindow)
    {
        projectSettingsWindow.update(windowFlags.projectSettingsWindow);
    }
    
    // Build Window
    if (windowFlags.buildWindow)
    {
        buildWindow.update(windowFlags.buildWindow);
    }

    // Diagnostic Window
    if (windowFlags.diagnosticWindow)
    {
        diagnosticWindow.update(windowFlags.diagnosticWindow);
    }
#pragma endregion

    // Status Bar
    {
        StatusBarInfo statusInfo;
        statusInfo.fps = ImGui::GetIO().Framerate;
        statusInfo.currentTool = "Select";
        statusBar.render(statusInfo);
    }


}

void enjinn::Editor::saveFlagsData()
{

	sfs::safeSave(&optionsFlags, sizeof(optionsFlags), ENJINN_ENGINE_SAVES_PATH "options", false);
	sfs::safeSave(&windowFlags, sizeof(windowFlags), ENJINN_ENGINE_SAVES_PATH "window", false);

    assetManagerWindow.saveData();


}

#endif

