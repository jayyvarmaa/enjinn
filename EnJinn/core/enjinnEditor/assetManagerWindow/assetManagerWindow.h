#pragma once

#include "enjinnConfig.h"
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include <logs/log.h>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <enjinnImgui/enjinnImgui.h>
#include <filesystem>
#include <string>
#include <containerManager/containerManager.h>
#include <unordered_map>

namespace enjinn
{

	struct TextureInfo
	{
		unsigned int id = 0;
		int w = 0;
		int h = 0;
	};


	struct AssetManagerWindow
	{

		void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);

		void update(bool &open, ContainerManager &containerManager, LoadedDll &currentDll,
			enjinn::LogManager &logManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager, ConsoleWindow *consoleWindow);

		static constexpr char *ICON = ICON_FK_FILES_O;
		static constexpr char *NAME = "Asset manager";
		static constexpr char *ICON_NAME = ICON_FK_FILES_O " Asset manager";

		int imguiId = 0;
		char searchText[100] = {};

		std::filesystem::path currentPath = ENJINN_RESOURCES_PATH;
		
		std::unordered_map<std::string, TextureInfo> textureCache;

		// --- Overhaul State ---
		enum class ViewMode { Grid, List };
		enum class SortMode { Name, Date, Size, Type };

		struct FileEntry
		{
			std::string name;
			std::string extension;
			std::string path;
			bool isDirectory;
			uintmax_t size;
            // storing time as simple comparison value or string? 
            // filesystem::file_time_type is hard to format directly, let's keep it simple or store string
            std::string dateStr; 
            auto lastWriteTime() const { return std::filesystem::last_write_time(path); }
		};

		std::vector<FileEntry> currentFiles;
		bool needsRefresh = true;
		
		ViewMode currentViewMode = ViewMode::Grid;
		SortMode currentSortMode = SortMode::Name;
		bool sortAscending = true;
		float gridIconSize = 100.0f; 
		float listIconSize = 40.0f;

		std::string clipboardOperation = ""; // "" or "copy"
		std::string clipboardFile = "";

		void refreshFiles();
		void sortFiles();
        
        void saveData();
        void loadData();

		// Image Viewer State
		bool showImageViewer = false;
		std::string currentImageName;
		unsigned int currentTextureId = 0;
		int currentTextureW = 0;
		int currentTextureH = 0;
		float imageZoom = 1.0f;
		ImVec2 imageOffset = { 0.0f, 0.0f };
		bool isDraggingImage = false;
		ImVec2 lastMouseDragPos = { 0.0f, 0.0f };
	};

}

#endif