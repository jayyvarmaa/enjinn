#pragma once
#include <logs/log.h>
#include <runtimeContainer/runtimeContainer.h>

#include <unordered_map>
#include <string>

#include <dllLoader/dllLoader.h>

#include <windowSystemm/input.h>
#include <windowSystemm/window.h>

#include <enjinnImgui/enjinnImgui.h>
#include <enjinnConsoleManager/enjinnConsoleWindow.h>

namespace enjinn
{
	using containerId_t = unsigned int;
	
	struct ContainerManager
	{

		std::unordered_map<containerId_t, enjinn::RuntimeContainer> runningContainers;
		
		containerId_t createContainer(
			enjinn::ContainerInformation containerInformation,
			enjinn::LoadedDll &loadedDll, enjinn::LogManager &logManager, 
			enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager, ConsoleWindow *consoleWindow, std::string &cmd, size_t memoryPos = 0);

		containerId_t createContainer(
			std::string containerName,
			enjinn::LoadedDll &loadedDll, enjinn::LogManager &logManager,
			enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager, ConsoleWindow *consoleWindow, std::string &cmd, size_t memoryPos = 0);

		bool setSnapshotToContainer(enjinn::containerId_t containerId, const char* snapshotName,
			enjinn::LogManager &logManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIdManager);

		bool setRecordingToContainer(enjinn::containerId_t containerId, const char *recordingName,
			enjinn::LogManager &logManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIdManager);

		void* allocateContainerMemory(enjinn::RuntimeContainer &container, enjinn::ContainerInformation containerInformation, void *memPos = 0);

		//buffer should have the correct size
		void allocateContainerMemoryAtBuffer(enjinn::RuntimeContainer &container,
			enjinn::ContainerInformation containerInformation, void *buffer);


		//deallocates memory, does not call destructors
		void freeContainerStuff(enjinn::RuntimeContainer &container);

		void init();

		void update(
			enjinn::LoadedDll &loadedDll,
			enjinn::EnJinnWindow &window,
			enjinn::LogManager &logs,
			enjinn::enjinnImgui::ImGuiIdsManager &imguiIdManager,
			enjinn::ConsoleWindow *consoleWindow);

		bool reloadDll(enjinn::LoadedDll &loadedDll,
			enjinn::EnJinnWindow &window,
			enjinn::LogManager &logs);

		bool destroyContainer(containerId_t id, enjinn::LoadedDll &loadedDll,
			enjinn::LogManager &logManager);

		bool makeSnapshot(containerId_t id, enjinn::LogManager &logManager, const char* fileName);

		bool startRecordingContainer(containerId_t id, enjinn::LogManager &logManager, const char *fileName);

		bool stopRecordingContainer(containerId_t id, enjinn::LogManager &logManager);

		bool makeRecordingStep(containerId_t id, enjinn::LogManager &logManager, 
			enjinn::Input &input);

		//same as destroy container but doesn't call user destructors
		bool forceTerminateContainer(containerId_t id, enjinn::LoadedDll &loadedDll,
			enjinn::LogManager &logManager);

		void destroyAllContainers(enjinn::LoadedDll &loadedDll,
			enjinn::LogManager &logManager);

		containerId_t idCounter = 0;

		//todo move outside
		void *allocateOSMemory(size_t size, void* baseAdress = 0);

		void deallocateOSMemory(void *baseAdress);
	};


	std::vector<std::string> getAvailableSnapshots(enjinn::RuntimeContainer &info);
	std::vector<std::string> getAvailableRecordings(enjinn::RuntimeContainer &info);

	std::vector<std::string> getAvailableSnapshotsAnyMemoryPosition(enjinn::RuntimeContainer &info);
	std::vector<std::string> getAvailableRecordingAnyMemoryPosition(enjinn::RuntimeContainer &info);

	std::vector<std::string> getAvailableSnapshotsAnyMemoryPosition(enjinn::ContainerInformation &info);
	std::vector<std::string> getAvailableRecordingsAnyMemoryPosition(enjinn::ContainerInformation &info);

	bool checkIfSnapshotIsCompatible(enjinn::RuntimeContainer &info, const char *snapshotName);

	bool checkIfSnapshotIsCompatibleAnyMemoryPosition(enjinn::RuntimeContainer &info, const char *snapshotName);

	bool checkIfSnapshotIsCompatibleAnyMemoryPosition(enjinn::ContainerInformation &info, const char *snapshotName);

	void *getSnapshotMemoryPosition(const char *snapshotName);

}