#include "assetManagerWindow.h"
#include "enjinnConfig.h"

#if ENJINN_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif
#include <safeSave/safeSave.h>

#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>

#if !ENJINN_SHOULD_REMOVE_EDITOR

namespace enjinn
{

	void AssetManagerWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
	{
		imguiId = idManager.getImguiIds();
	}
	
	void AssetManagerWindow::refreshFiles()
	{
		currentFiles.clear();
		try
		{
			if (std::filesystem::exists(currentPath) && std::filesystem::is_directory(currentPath))
			{
				for (const auto& entry : std::filesystem::directory_iterator(currentPath))
				{
					FileEntry e;
					e.path = entry.path().string();
					e.name = entry.path().filename().string();
					e.extension = entry.path().extension().string();
					e.isDirectory = entry.is_directory();
					
					try {
						e.size = e.isDirectory ? 0 : entry.file_size();
					} catch (...) { e.size = 0; }

					// Date formatting (simple string for now)
					auto ftime = entry.last_write_time();
					auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
					std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
					char dateBuf[26];
					ctime_s(dateBuf, sizeof(dateBuf), &cftime);
					e.dateStr = dateBuf; 
					if(!e.dateStr.empty() && e.dateStr.back() == '\n') e.dateStr.pop_back();

					currentFiles.push_back(e);
				}
			}
		}
		catch (...) {}
		sortFiles();
		needsRefresh = false;
	}

	void AssetManagerWindow::sortFiles()
	{
		std::sort(currentFiles.begin(), currentFiles.end(), [this](const FileEntry& a, const FileEntry& b) {
			
			// Always put directories first? Or respect sort?
			// Windows puts folders first usually.
			if (a.isDirectory != b.isDirectory)
				return a.isDirectory > b.isDirectory; 

			bool ret = false;
			switch (currentSortMode)
			{
			case SortMode::Name: ret = a.name < b.name; break;
			case SortMode::Date: ret = a.lastWriteTime() < b.lastWriteTime(); break;
			case SortMode::Size: ret = a.size < b.size; break;
			case SortMode::Type: ret = a.extension < b.extension; break;
			}
			return sortAscending ? ret : !ret;
		});
	}
	
	void AssetManagerWindow::update(bool &open, ContainerManager &containerManager, LoadedDll &currentDll,
		enjinn::LogManager &logManager, enjinn::enjinnImgui::ImGuiIdsManager &imguiIDsManager, ConsoleWindow *consoleWindow)
	{
		if (needsRefresh) refreshFiles();

		ImGui::PushID(imguiId);

		// Set default window size for first use
		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

		if (!ImGui::Begin(ICON_NAME, &open))
		{
			ImGui::End();
			ImGui::PopID();
			return;
		}


		// --- Navigation & Toolbar ---
		if(std::filesystem::equivalent(currentPath, ENJINN_RESOURCES_PATH) || searchText[0] != '\0')
		{
			ImGui::BeginDisabled(1);
		}
		else
		{
			ImGui::BeginDisabled(0);
		}

		if (ImGui::Button(ICON_FK_ARROW_UP))
		{
			currentPath = currentPath.parent_path();
			needsRefresh = true;
		}

		ImGui::EndDisabled();

		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if(ImGui::InputText("Search", searchText, sizeof(searchText)))
		{
			// Filter logic handled in loop or we filter vector?
			// For simplicity, we just filter display execution, reusing old logic style but adapted.
		}

		ImGui::SameLine();
		if (ImGui::Button("Refresh"))
		{
			needsRefresh = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Folder"))
		{
		#if ENJINN_WINDOWS
			ShellExecuteA(NULL, "open", currentPath.string().c_str(), NULL, NULL, SW_RESTORE);
		#endif
		}
		
		// --- View Options ---
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::BeginCombo("Sort", (currentSortMode == SortMode::Name ? "Name" : 
									  currentSortMode == SortMode::Date ? "Date" :
									  currentSortMode == SortMode::Size ? "Size" : "Type")))
		{
			if(ImGui::Selectable("Name")) { currentSortMode = SortMode::Name; sortFiles(); }
			if(ImGui::Selectable("Date")) { currentSortMode = SortMode::Date; sortFiles(); }
			if(ImGui::Selectable("Size")) { currentSortMode = SortMode::Size; sortFiles(); }
			if(ImGui::Selectable("Type")) { currentSortMode = SortMode::Type; sortFiles(); }
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if(ImGui::Checkbox("Asc", &sortAscending)) sortFiles();

		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		if (ImGui::BeginCombo("View", (currentViewMode == ViewMode::Grid ? "Grid" : "List")))
		{
			if(ImGui::Selectable("Grid")) currentViewMode = ViewMode::Grid;
			if(ImGui::Selectable("List")) currentViewMode = ViewMode::List;
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
        
        float* currentIconSizePtr = (currentViewMode == ViewMode::Grid) ? &gridIconSize : &listIconSize;
		ImGui::SliderFloat("Size", currentIconSizePtr, 32.0f, 256.0f, "%.0f");

        // Mouse Back Button Navigation (Button 3)
        if (ImGui::IsMouseClicked(3))
        {
            if (currentPath.has_parent_path() && !std::filesystem::equivalent(currentPath, ENJINN_RESOURCES_PATH))
            {
                currentPath = currentPath.parent_path();
                needsRefresh = true;
            }
        }

		// --- Breadcrumbs ---
		std::string longPath = currentPath.string();
		std::string root = ENJINN_RESOURCES_PATH;
		std::string enginePath = "ENJINN_RESOURCES_PATH/";
		std::string copyPath = "";
		if (longPath.size() > root.size())
		{
			enginePath += (longPath.c_str() + root.size());
			copyPath += (longPath.c_str() + root.size());
		}
		// Fix slashes
		for (char &c : enginePath) { if (c == '\\') c = '/'; }
		ImGui::Text(enginePath.c_str());

		ImGui::Separator();

		// --- File Operations (Context on Window) ---
		if (ImGui::BeginPopupContextWindow("WindowContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("New Folder"))
			{
				std::filesystem::create_directory(currentPath / "New Folder");
				needsRefresh = true;
			}
			if (ImGui::MenuItem("Paste", nullptr, false, !clipboardFile.empty() && clipboardOperation == "copy"))
			{
				try {
					std::filesystem::copy(clipboardFile, currentPath / std::filesystem::path(clipboardFile).filename());
					needsRefresh = true;
				} catch(...) {}
			}
			ImGui::EndPopup();
		}

		// --- MAIN CONTENT ---
		ImGui::BeginChild("AssetContent", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
		
		float contentW = ImGui::GetContentRegionAvail().x;
        const float padding = 10.0f;
		
		auto DrawFileIcon = [&](const FileEntry& file, bool listView = false)
		{
			// Check extension for image tex
			bool isImage = (file.extension == ".png" || file.extension == ".jpg" || file.extension == ".jpeg" || file.extension == ".bmp" || file.extension == ".tga");
			ImTextureID texID = 0;
			
			if(file.isDirectory) {}
			else if(isImage)
			{
				if (textureCache.find(file.path) == textureCache.end())
				{
					// Load texture
					int w, h, channels;
					unsigned char* data = stbi_load(file.path.c_str(), &w, &h, &channels, 4);
					if (data)
					{
						GLuint id = 0;
						glGenTextures(1, &id);
						glBindTexture(GL_TEXTURE_2D, id);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
						glGenerateMipmap(GL_TEXTURE_2D);
						textureCache[file.path] = { id, w, h };
						stbi_image_free(data);
					}
					else textureCache[file.path] = {0,0,0};
				}
				texID = (ImTextureID)(size_t)textureCache[file.path].id;
			}
			
			ImGui::PushID(file.path.c_str());
			
			bool hasTexture = (texID != 0);
			float useSize = *currentIconSizePtr; 
			if(listView) useSize = *currentIconSizePtr;

			if(hasTexture)
			{
				TextureInfo& info = textureCache[file.path];
				float scale = 1.0f;
				if(info.w > 0 && info.h > 0)
				{
					scale = std::min(useSize / (float)info.w, useSize / (float)info.h);
				}
				
				float w = info.w * scale;
				float h = info.h * scale;
				
				if(!listView)
				{
					float PadX = (useSize - w) * 0.5f;
					float PadY = (useSize - h) * 0.5f;
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + PadX);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + PadY);
				}
				
				// Fix: UVs (0,0) to (1,1) for standard GL texture orientation (if stbi didn't flip)
                // If previous was (0,1)->(1,0) (Upside down), then flipping V should work.
                // Assuming stbi raw data + GL default = Upside Down logic.
                // Let's try standard UVs.
				if(ImGui::ImageButton(texID, {w, h}, ImVec2(0, 1), ImVec2(1, 0)))
				{
					if(isImage)
					{
                        currentTextureId = info.id;
                        currentTextureW = info.w;
                        currentTextureH = info.h;
                        currentImageName = file.name;
                        imageZoom = 1.0f;
                        imageOffset = { 0.0f, 0.0f };
                        showImageViewer = true;
                        ImGui::OpenPopup("ImageViewerModal");
					}
				}
			}
			else
			{
				const char* icon = file.isDirectory ? ICON_FK_FOLDER_O : ICON_FK_FILE_O;
				if(ImGui::Button(icon, {useSize, useSize}))
				{
					if(file.isDirectory)
					{
						currentPath = file.path;
						needsRefresh = true;
					}
					else
					{
                        auto it = currentDll.containerExtensionsSupport.find(file.extension);
                        if (it != currentDll.containerExtensionsSupport.end())
                        {
                            std::string pathCopy = file.path;
                            containerManager.createContainer(it->second[0], currentDll, logManager, imguiIDsManager, consoleWindow, pathCopy);
                        }
					}
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("FILES_PAYLOAD", file.path.c_str(), file.path.size() + 1);
				ImGui::Text(file.name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Copy Path")) { ImGui::SetClipboardText(file.path.c_str()); }
				if (ImGui::MenuItem("Copy File")) { clipboardFile = file.path; clipboardOperation = "copy"; }
				if (ImGui::MenuItem("Delete")) 
				{ 
					try { std::filesystem::remove(file.path); needsRefresh = true; } catch(...) {}
				}
				ImGui::EndPopup();
			}

			ImGui::PopID();
			if(!listView) ImGui::TextWrapped(file.name.c_str());
		};



		if (currentViewMode == ViewMode::Grid)
		{
			// Grid View
			float cellSize = gridIconSize + padding;
			int columns = (int)(contentW / cellSize);
			if (columns < 1) columns = 1;
			
			ImGui::Columns(columns, 0, false);
			
			for (const auto& file : currentFiles)
			{
				if (searchText[0] != '\0' && file.name.find(searchText) == std::string::npos) continue;
				DrawFileIcon(file, false);
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}
		else
		{
			// List View
            // We keep default font for Headers so they don't get huge.
            // Scaling is applied INSIDE the loop.

			if (ImGui::BeginTable("FilesTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Date");
				ImGui::TableSetupColumn("Type");
				ImGui::TableSetupColumn("Size");
				ImGui::TableHeadersRow();

				for (const auto& file : currentFiles)
				{
					if (searchText[0] != '\0' && file.name.find(searchText) == std::string::npos) continue;

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					
                    // --- Safe Font Selection ---
                    auto& fonts = ImGui::GetIO().Fonts->Fonts;
                    ImFont* textFont = fonts[0]; 
                    ImFont* iconFont = (fonts.Size > 1) ? fonts[1] : fonts[0]; 

                    if (fonts.Size >= 4) {
                        iconFont = fonts[2]; 
                        textFont = fonts[3]; 
                    } else if (fonts.Size >= 3) {
                        textFont = fonts[fonts.Size - 1]; 
                        iconFont = (fonts.Size > 1) ? fonts[1] : fonts[0]; 
                    }

                    // --- Name & Icon ---
                    // --- Name & Icon ---
                float contentHeight = std::max(15.0f, listIconSize * 0.8f); 

                    // 1. Draw Icon
                    ImGui::PushFont(iconFont); 
                    float iconBase = (iconFont->FontSize > 50.0f) ? 100.0f : 14.0f; 
                    ImGui::SetWindowFontScale(contentHeight / iconBase);
                    
                    const char* icon = file.isDirectory ? ICON_FK_FOLDER_O : ICON_FK_FILE_O;
                    if(file.isDirectory) ImGui::TextColored({1,1,0,1}, icon);
                    else ImGui::Text(icon);
                    
                    ImGui::PopFont(); 
                    ImGui::SetWindowFontScale(1.0f); 

                    ImGui::SameLine();
                    
                    // 2. Draw Text
                    ImGui::PushFont(textFont); 
                    float textBase = (textFont->FontSize > 50.0f) ? 64.0f : 15.0f;
                    ImGui::SetWindowFontScale(contentHeight / textBase);
                    
                    if(ImGui::Selectable(file.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
                    {
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            if(file.isDirectory) { currentPath = file.path; needsRefresh = true; }
                            else 
                            { 
                                // File Open logic
                                // Check if image -> viewer
                                // Check if extension -> load
                                bool isImage = (file.extension == ".png" || file.extension == ".jpg" || file.extension == ".jpeg" || file.extension == ".bmp" || file.extension == ".tga");
                                if(isImage) {
                                    if(textureCache.find(file.path) != textureCache.end())
                                    {
                                        TextureInfo& info = textureCache[file.path];
                                        currentTextureId = info.id;
                                        currentTextureW = info.w;
                                        currentTextureH = info.h;
                                        currentImageName = file.name;
                                        imageZoom = 1.0f;
                                        imageOffset = { 0.0f, 0.0f };
                                        showImageViewer = true;
                                        ImGui::OpenPopup("ImageViewerModal");
                                    }
                                }
                            }
                        }
                    }
                    
                    ImGui::PopFont(); 
                    ImGui::SetWindowFontScale(1.0f); 

                    // Drag Source in List
                    if (ImGui::BeginDragDropSource())
                    {
                        ImGui::SetDragDropPayload("FILES_PAYLOAD", file.path.c_str(), file.path.size() + 1);
                        ImGui::Text(file.name.c_str());
                        ImGui::EndDragDropSource();
                    }

					ImGui::TableNextColumn();
                    ImGui::PushFont(textFont);
                    ImGui::SetWindowFontScale(contentHeight / textBase);
					ImGui::Text(file.dateStr.c_str());
                    ImGui::PopFont(); ImGui::SetWindowFontScale(1.0f);

					ImGui::TableNextColumn();
                    ImGui::PushFont(textFont);
                    ImGui::SetWindowFontScale(contentHeight / textBase);
					ImGui::Text(file.extension.c_str());
                    ImGui::PopFont(); ImGui::SetWindowFontScale(1.0f);

					ImGui::TableNextColumn();
                    ImGui::PushFont(textFont);
                    ImGui::SetWindowFontScale(contentHeight / textBase);
					if(!file.isDirectory) ImGui::Text("%ju KB", file.size / 1024);
                    ImGui::PopFont(); ImGui::SetWindowFontScale(1.0f);
				}
				ImGui::EndTable();
			}
		}

        ImGui::EndChild(); // End Content Area

		ImGui::Columns(1);

    // Image Viewer Logic
    if (showImageViewer)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        // Fix: Removed ImGuiWindowFlags_NoMove so window IS movable via title bar
        if (ImGui::BeginPopupModal("ImageViewerModal", &showImageViewer, 
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            if (currentTextureId != 0)
            {
                // UI Header
                ImGui::Text("%s (%dx%d)", currentImageName.c_str(), currentTextureW, currentTextureH);
                ImGui::SameLine();
                if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape))
                {
                    showImageViewer = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::Separator();

                // Canvas Area (Rest of the window)
                ImVec2 availRegion = ImGui::GetContentRegionAvail();
                ImVec2 canvasPos = ImGui::GetCursorScreenPos();
                
                // Create an invisible button to act as the canvas. 
                // This captures mouse interactions (preventing window move on drag) and defines the area.
                ImGui::InvisibleButton("##img_canvas", availRegion);
                
                // Canvas Info - needed for zoom calculation and rendering
                ImVec2 centerPos = { 
                    canvasPos.x + availRegion.x * 0.5f, 
                    canvasPos.y + availRegion.y * 0.5f 
                };
                
                // Calculate Draw Info EARLY for input check
                ImVec2 drawSize = { (float)currentTextureW * imageZoom, (float)currentTextureH * imageZoom };
                ImVec2 drawPos = {
                    centerPos.x - drawSize.x * 0.5f + imageOffset.x,
                    centerPos.y - drawSize.y * 0.5f + imageOffset.y
                };

                // Logic for Scroll Zoom (on hover)
                if (ImGui::IsItemHovered())
                {
                    float wheel = ImGui::GetIO().MouseWheel;
                    // Fix: Check if mouse is strictly inside the image bounds
                    ImVec2 mousePos = ImGui::GetMousePos();
                    bool isMouseOverImage = (mousePos.x >= drawPos.x && mousePos.x <= drawPos.x + drawSize.x &&
                                             mousePos.y >= drawPos.y && mousePos.y <= drawPos.y + drawSize.y);

                    if (wheel != 0 && isMouseOverImage)
                    {
                        float oldZoom = imageZoom;
                        float zoomSpeed = 0.1f;
                        if(imageZoom > 1.0f) zoomSpeed = 0.2f;
                        if(imageZoom > 5.0f) zoomSpeed = 0.5f;
                        
                        imageZoom += wheel * zoomSpeed;
                        if (imageZoom < 0.1f) imageZoom = 0.1f;
                        if (imageZoom > 20.0f) imageZoom = 20.0f;

                        // Zoom towards cursor logic
                        // RelMouse = Mouse - Center
                        // NewOffset = RelMouse - (RelMouse - OldOffset) * (NewZoom / OldZoom)
                        
                        // Recalculate relMouse based on centerPos (canvas center)
                        ImVec2 relMouse = { mousePos.x - centerPos.x, mousePos.y - centerPos.y };
                        
                        float scale = imageZoom / oldZoom;
                        
                        imageOffset.x = relMouse.x - (relMouse.x - imageOffset.x) * scale;
                        imageOffset.y = relMouse.y - (relMouse.y - imageOffset.y) * scale;
                        
                        // Re-calculate draw info for this frame rendering ?? 
                        // Actually we draw with the NEW zoom, so we should update drawPos/Size for rendering below?
                        // Yes, let's update local vars for rendering.
                        drawSize = { (float)currentTextureW * imageZoom, (float)currentTextureH * imageZoom };
                        drawPos = {
                            centerPos.x - drawSize.x * 0.5f + imageOffset.x,
                            centerPos.y - drawSize.y * 0.5f + imageOffset.y
                        };
                    }
                }

                // Logic for Pan (on active/drag)
                // Pan can happen even if outside image? User said "if my mouse pointer is not on the image itself no zomming will be performed"
                // Assuming panning is still fine anywhere on canvas or maybe just image? usually canvas is fine.
                if (ImGui::IsItemActive() && (ImGui::IsMouseDragging(ImGuiMouseButton_Middle) || ImGui::IsMouseDragging(ImGuiMouseButton_Left)))
                {
                    ImVec2 delta = ImGui::GetIO().MouseDelta;
                    imageOffset.x += delta.x;
                    imageOffset.y += delta.y;
                    
                    // Update drawPos for rendering
                    drawPos.x += delta.x;
                    drawPos.y += delta.y;
                }

                // Render Image (Clipped to Canvas)
                ImGui::PushClipRect(canvasPos, ImVec2(canvasPos.x + availRegion.x, canvasPos.y + availRegion.y), true);
                
                // Draw using calculated values
                ImGui::GetWindowDrawList()->AddImage(
                    (void*)(intptr_t)currentTextureId,
                    drawPos,
                    { drawPos.x + drawSize.x, drawPos.y + drawSize.y },
                    { 0, 1 }, { 1, 0 } 
                );
                
                ImGui::PopClipRect();
            }
            ImGui::EndPopup();
        }
    }
    
    // Ensure popup is kept open if flag is true, but check if we need to open it
    if(showImageViewer && !ImGui::IsPopupOpen("ImageViewerModal"))
    {
       ImGui::OpenPopup("ImageViewerModal");
    }

		ImGui::End();
		ImGui::PopID();
	}

    struct AssetManagerSaveData
    {
        float gridIconSize;
        float listIconSize;
        int currentViewMode;
    };
	void AssetManagerWindow::saveData()
	{
        AssetManagerSaveData data;
        data.gridIconSize = gridIconSize;
        data.listIconSize = listIconSize;
        data.currentViewMode = (int)currentViewMode;
        
		sfs::safeSave(&data, sizeof(data), ENJINN_ENGINE_SAVES_PATH "assetManagerData", false);
	}

	void AssetManagerWindow::loadData()
	{
        AssetManagerSaveData data;
		if (sfs::safeLoad(&data, sizeof(data), ENJINN_ENGINE_SAVES_PATH "assetManagerData", false) == sfs::noError)
        {
            gridIconSize = data.gridIconSize;
            listIconSize = data.listIconSize;
            currentViewMode = (ViewMode)data.currentViewMode;
        }
	}

}


#endif