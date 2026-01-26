#pragma once

#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <logs/log.h>
#include <dllLoader/dllLoader.h>
#include <containerManager/containerManager.h>

namespace enjinn
{

	struct ContainersWindow
	{

		void init(enjinn::enjinnImgui::ImGuiIdsManager &imguiIdsManager);

		void update(enjinn::LogManager &logManager, bool &open, enjinn::LoadedDll &loadedDll,
			enjinn::ContainerManager &containerManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIdsManager,
			enjinn::ConsoleWindow *consoleWindow);

		static constexpr char *ICON = ICON_FK_MICROCHIP;
		static constexpr char *NAME = "Containers manager";
		static constexpr char *ICON_NAME = ICON_FK_MICROCHIP " Containers manager";

		char filterContainerInfo[50] = {};
		char filterSnapshots[50] = {};
		char snapshotName[50] = {};
		char recordingName[50] = {};

		int itemCurrentAvailableCOntainers = 0;
		int itemCurrentCreatedContainers = 0;
		int currentSelectedSnapshot = 0;
		int currentSelectedRecording = 0;

		bool createAtSpecificMemoryRegion = 0;

		int imguiIds = 0;
	};



}

#endif