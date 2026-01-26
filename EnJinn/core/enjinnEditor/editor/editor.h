#pragma once
//////////////////////////////////////////
//editor.h
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////


#include <enjinnConfig.h>

#if !ENJINN_SHOULD_REMOVE_EDITOR


#include <enjinnImgui/enjinnImgui.h>
#include <gl2d/gl2d.h>
#include <logs/logWindow.h>
#include <windowSystemm/input.h>
#include <shortcutApi/shortcutApi.h>
#include <editShortcuts/editShortcuts.h>
#include <pushNotification/pushNotification.h>
#include <containersWindow/containersWindow.h>
#include <enjinnConsoleManager/enjinnConsoleWindow.h>
#include <assetManagerWindow/assetManagerWindow.h>
#include <openglLogs/openglLogs.h>
#include "hierarchyWindow.h"
#include "inspectorWindow.h"
#include "sceneViewWindow.h"
#include <phaseManager/phaseManager.h>

namespace enjinn
{

	struct Editor
	{

		void init(enjinn::ShortcutManager &shortcutManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDManager);

		void update(const enjinn::Input &input, enjinn::ShortcutManager &shortcutManager
			,enjinn::LogManager &logs, enjinn::PushNotificationManager &pushNotificationManager,
			enjinn::LoadedDll &loadedDll, enjinn::ContainerManager &containerManager,
			enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager);

		void saveFlagsData();

		struct
		{
			bool hideMainWindow = 0;
		}optionsFlags;

		struct
		{
			bool logsWindow = 0;
			bool editShortcutsWindow = 0;
			bool containerManager = 0;
			bool transparentWindow = 0;
			bool consoleWindow = 0;
			bool assetManagerWindow = 0;

			bool openglErrorsWindow = 0;
            bool hierarchyWindow = 1;
            bool inspectorWindow = 1;
            bool sceneViewWindow = 1;
		}windowFlags;

		enjinn::LogWindow logWindow;
		enjinn::EditShortcutsWindow editShortcutsWindow;
		enjinn::ContainersWindow containersWindow;
		enjinn::ConsoleWindow consoleWindow;
		enjinn::AssetManagerWindow assetManagerWindow;
		enjinn::OpenglLogsWindow openglLogsWindow;
        enjinn::HierarchyWindow hierarchyWindow;
        enjinn::InspectorWindow inspectorWindow;
        enjinn::SceneViewWindow sceneViewWindow;

		bool lastHideWindowState = optionsFlags.hideMainWindow;

		bool shouldReloadDll = 0;
		int imguiId = 0;
		gl2d::Texture logoTexture = {};
		char secretCodeBuffer[64] = {};  // SECRET: Phase unlock
	};



}

#endif