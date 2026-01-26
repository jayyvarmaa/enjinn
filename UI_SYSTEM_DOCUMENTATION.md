# EnJinn UI System Documentation

**Complete Technical Reference for the EnJinn Game Engine User Interface**

---

## Table of Contents

1. [Overview](#overview)
2. [UI Framework - Dear ImGui](#ui-framework---dear-imgui)
3. [Theme & Styling](#theme--styling)
4. [Fonts & Icons](#fonts--icons)
5. [Editor Windows](#editor-windows)
6. [Custom UI Components](#custom-ui-components)
7. [Layout System](#layout-system)
8. [Menu System](#menu-system)
9. [Shortcuts & Keybindings](#shortcuts--keybindings)
10. [Memory Management](#memory-management)
11. [Code Examples & Snippets](#code-examples--snippets)

---

## Overview

EnJinn uses **Dear ImGui (Docking Branch)** as its UI framework, providing a modern, Unity-inspired editor interface with extensive customization capabilities. The entire UI system is designed to be modular, extensible, and optimized for game development workflows.

### Key Features

- ✅ **Docking Support** - Full window docking like Unity/Unreal
- ✅ **Multi-Viewport** - Detach windows to separate OS windows
- ✅ **Custom Theme** - Unity-inspired dark theme with blue accents
- ✅ **Icon Integration** - FontAwesome icons throughout
- ✅ **Hot Reload** - UI persists through DLL reloads
- ✅ **Layout System** - Save/load custom window layouts  
- ✅ **Keyboard Navigation** - Full keyboard controls

**File Location**: `core/sharedRuntime/enjinnImgui/` and `core/enjinnEditor/`

---

## UI Framework - Dear ImGui

### Initialization

```cpp
// File: core/sharedRuntime/enjinnImgui/enjinnImgui.cpp (Line 48-206)

void enjinn::enjinnImgui::initImgui(EnJinnContext &enjinnContext)
{
    // Set custom allocator for memory management
    setImguiAllocator(enjinnContext.imguiAllocator);

    // Enable Windows dark mode title bar
    HWND hwnd = glfwGetWin32Window(enjinnContext.wind);
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

    // Create ImGui context
    auto context = ::ImGui::CreateContext();
    
    // Configure ImGui flags
    ImGuiIO &io = ::ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Docking ✓
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Multi-viewport ✓
    io.FontGlobalScale = 1.0f;                                // 2K display scaling
    
    // Initialize GLFW and OpenGL backends
    ImGui_ImplGlfw_InitForOpenGL(enjinnContext.wind, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
```

### Frame Management

```cpp
// Start Frame (Line 215-222)
void enjinn::enjinnImgui::imguiStartFrame(EnJinnContext enjinnContext)
{
    setImguiContext(enjinnContext);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ::ImGui::NewFrame();
    ::ImGui::DockSpaceOverViewport(::ImGui::GetMainViewport());  // Global dockspace
}

// End Frame (Line 226-256)
void enjinn::enjinnImgui::imguiEndFrame(EnJinnContext enjinnContext)
{
    setImguiContext(enjinnContext);
    ::ImGui::Render();
    
    // Render to viewport
    int display_w = 0, display_h = 0;
    glfwGetFramebufferSize(enjinnContext.wind, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
    
    // Multi-viewport support
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ::ImGui::UpdatePlatformWindows();
        ::ImGui::RenderPlatformWindowsDefault();
    }
}
```

---

## Theme & Styling

### Color Scheme - Unity-Inspired Dark Theme

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 61-137)

```cpp
// Modern Flat Theme - Unity Inspired
ImGuiStyle& style = ImGui::GetStyle();

// ===== ROUNDING =====
style.WindowRounding = 4.0f;
style.ChildRounding = 4.0f;
style.FrameRounding = 3.0f;
style.GrabRounding = 3.0f;
style.PopupRounding = 4.0f;
style.ScrollbarRounding = 6.0f;
style.TabRounding = 4.0f;
```

### Spacing & Padding

```cpp
// Better padding and spacing
style.WindowPadding = ImVec2(8, 8);      // Space inside windows
style.FramePadding = ImVec2(6, 4);       // Space inside frames
style.ItemSpacing = ImVec2(8, 6);        // Space between items
style.ItemInnerSpacing = ImVec2(6, 4);   // Space inside items
style.IndentSpacing = 20.0f;             // Tree indentation
style.ScrollbarSize = 14.0f;             // Scrollbar width
style.GrabMinSize = 12.0f;               // Minimum grab size
```

### Color Palette

**Background Colors:**
```cpp
colors[ImGuiCol_WindowBg]         = ImVec4(0.13f, 0.13f, 0.13f, 1.00f); // Dark gray
colors[ImGuiCol_ChildBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // Transparent
colors[ImGuiCol_PopupBg]          = ImVec4(0.16f, 0.16f, 0.16f, 0.95f); // Slightly lighter
colors[ImGuiCol_MenuBarBg]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f); // Menu bar
colors[ImGuiCol_TitleBg]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Title (inactive)
colors[ImGuiCol_TitleBgActive]    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f); // Title (active)
```

**Interactive Elements:**
```cpp
// Blue accent color (0.26f, 0.59f, 0.98f) used throughout:
colors[ImGuiCol_CheckMark]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Checkboxes
colors[ImGuiCol_SliderGrab]       = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Sliders
colors[ImGuiCol_ButtonActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Clicked buttons
colors[ImGuiCol_Header]           = ImVec4(0.22f, 0.22f, 0.22f, 1.00f); // Tree nodes
colors[ImGuiCol_HeaderHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Hover effect
colors[ImGuiCol_HeaderActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Selected
```

**Buttons:**
```cpp
colors[ImGuiCol_Button]           = ImVec4(0.22f, 0.22f, 0.22f, 1.00f); // Default
colors[ImGuiCol_ButtonHovered]    = ImVec4(0.28f, 0.28f, 0.28f, 1.00f); // Hover
colors[ImGuiCol_ButtonActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Pressed (blue!)
```

**Text:**
```cpp
colors[ImGuiCol_Text]             = ImVec4(0.92f, 0.92f, 0.92f, 1.00f); // Almost white
colors[ImGuiCol_TextDisabled]     = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Gray
colors[ImGuiCol_TextSelectedBg]   = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // Blue with alpha
```

**Docking System:**
```cpp
colors[ImGuiCol_Tab]              = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Inactive tab
colors[ImGuiCol_TabHovered]       = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Hover tab
colors[ImGuiCol_TabActive]        = ImVec4(0.22f, 0.22f, 0.22f, 1.00f); // Active tab
colors[ImGuiCol_DockingPreview]   = ImVec4(0.26f, 0.59f, 0.98f, 0.70f); // Drop preview
colors[ImGuiCol_DockingEmptyBg]   = ImVec4(0.12f, 0.12f, 0.12f, 1.00f); // Empty dock area
```

**Borders & Separators:**
```cpp
colors[ImGuiCol_Border]           = ImVec4(0.25f, 0.25f, 0.25f, 0.50f); // Window borders
colors[ImGuiCol_Separator]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Lines
colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Hover
colors[ImGuiCol_SeparatorActive]  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Active
```

### Multi-Viewport Adjustments

```cpp
// Make windows transparent when detached (for multi-viewport)
if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
{
    style.Colors[ImGuiCol_WindowBg].w = 0.f;        // Transparent
    style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;  // Transparent
}
```

---

## Fonts & Icons

### Font Configuration

**Loaded Fonts**: (Lines 169-202)

1. **Primary Font: Arial**
   ```cpp
   // Size: 12px
   io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "arial.ttf", 12);
   ```
   - **File**: `EnJinn/resources/arial.ttf`
   - **Purpose**: Main UI text
   - **Size**: 12px (scales with `FontGlobalScale`)

2. **Icon Font: FontAwesome (Merged)**
   ```cpp
   // Merged into Arial font at size 12px
   ImFontConfig config;
   config.MergeMode = true;
   config.GlyphMinAdvanceX = 16.0f;  // Monospaced icons
   
   static const ImWchar icon_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
   io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "fontawesome-webfont.ttf", 
                                12.0f, &config, icon_ranges);
   ```
   - **File**: `EnJinn/resources/fontawesome-webfont.ttf`
   - **Purpose**: UI icons (merged with Arial)
   - **Range**: Full FontAwesome 4 icon set

3. **Large Icon Font** (Asset Manager)
   ```cpp
   // Size: 100px for file/folder icons
   ImVector<ImWchar> ranges;
   ImFontGlyphRangesBuilder builder;
   builder.AddChar(0xf016);  // ICON_FK_FILE_O
   builder.AddChar(0xf114);  // ICON_FK_FOLDER_O
   builder.BuildRanges(&ranges);
   
   io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "fontawesome-webfont.ttf", 
                                100.f / io.FontGlobalScale, 0, ranges.Data);
   ```
   - **Purpose**: Large file/folder icons in asset manager
   - **Size**: 100px
   - **Glyphs**: Only file and folder icons (optimized)

### Icon System - FontAwesome 4

**Header**: `IconsForkAwesome.h`

**Usage Example**:
```cpp
// Icons used in the editor (editor.cpp, Lines 21-32)
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
```

**Icon Reference**:
- 📁 `ICON_FK_FOLDER_O` - Folder
- 📄 `ICON_FK_FILE_O` - File
- 👁 `ICON_FK_EYE` / `ICON_FK_EYE_SLASH` - Show/Hide
- ⚙ `ICON_FK_COG` / `ICON_FK_COGS` - Settings
- 🎮 `ICON_FK_GAMEPAD` - Gameplay
- 🔧 `ICON_FK_PENCIL_SQUARE` - Edit
- 🔄 `ICON_FK_REFRESH` - Reload
- 📊 `ICON_FK_LIST` - Hierarchy
- ℹ️ `ICON_FK_INFO` - Inspector
- 🖼️ `ICON_FK_PICTURE_O` - Scene View

**Full list**: [FontAwesome 4 Icons](https://fontawesome.com/v4/icons/)

### Large Icons (Asset Manager)

**Usage**:
```cpp
// File: assetManagerWindow.cpp (Lines 103-115)
ImFontAtlas *atlas = ImGui::GetIO().Fonts;
ImGui::PushFont(atlas->Fonts[1]);  // Switch to large icon font

// Draw huge folder/file icons
ImGui::Text(ICON_FK_FOLDER_O);  // 100px folder icon

ImGui::PopFont();
```

---

## Editor Windows

The EnJinn editor consists of multiple specialized windows, all dockable and persistent.

### Window System Architecture

**File**: `core/enjinnEditor/editor/editor.h` (Lines 32-79)

```cpp
struct Editor
{
    // Window visibility flags
    struct
    {
        bool logsWindow = 0;
        bool editShortcutsWindow = 0;
        bool containerManager = 0;
        bool transparentWindow = 0;
        bool consoleWindow = 0;
        bool assetManagerWindow = 0;
        bool openglErrorsWindow = 0;
        bool hierarchyWindow = 1;      // Default ON
        bool inspectorWindow = 1;      // Default ON
        bool sceneViewWindow = 1;      // Default ON
    } windowFlags;

    // Window instances
    enjinn::LogWindow logWindow;
    enjinn::EditShortcutsWindow editShortcutsWindow;
    enjinn::ContainersWindow containersWindow;
    enjinn::ConsoleWindow consoleWindow;
    enjinn::AssetManagerWindow assetManagerWindow;
    enjinn::OpenglLogsWindow openglLogsWindow;
    enjinn::HierarchyWindow hierarchyWindow;
    enjinn::InspectorWindow inspectorWindow;
    enjinn::SceneViewWindow sceneViewWindow;
};
```

### 1. Hierarchy Window

**File**: `core/enjinnEditor/editor/hierarchyWindow.h`

**Purpose**: Scene graph tree view (like Unity's Hierarchy)

```cpp
struct HierarchyWindow
{
    static constexpr const char* ICON_NAME = ICON_FK_LIST " Hierarchy";
    
    Node* selectedNode = nullptr;       // Currently selected node
    char searchBuffer[256] = {};        // Search functionality
    
    void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
    void update(Scene* scene, bool &open);

private:
    int windowId = 0;
    void drawNode(Node* node);          // Recursive tree drawing
    
    Node* nodeToDestroy = nullptr;
    Node* dragSource = nullptr;         // Drag-and-drop support
    Node* dragTarget = nullptr;
};
```

**Features**:
- Tree view of scene nodes
- Node selection
- Search/filter
- Drag-and-drop reparenting
- Context menus for nodes

### 2. Inspector Window

**File**: `core/enjinnEditor/editor/inspectorWindow.h`

**Purpose**: Component properties editor (like Unity's Inspector)

```cpp
struct InspectorWindow
{
    static constexpr const char* ICON_NAME = ICON_FK_INFO " Inspector";
    
    void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
    void update(Node* selectedNode, bool &open);

private:
    int windowId = 0;
};
```

**Features**:
- Displays selected node properties
- Component editing
- Transform controls (position, rotation, scale)
- Custom component inspectors

### 3. Scene View Window

**File**: `core/enjinnEditor/editor/sceneViewWindow.h`

**Purpose**: Real-time game view (like Unity's Scene tab)

```cpp
struct SceneViewWindow
{
    static constexpr const char* ICON_NAME = ICON_FK_PICTURE_O " Scene View";
    
    void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
    void update(unsigned int textureId, bool &open);

private:
    int windowId = 0;
};
```

**Features**:
- Displays rendered game view
- Shows framebuffer texture from active container

### 4. Log Window

**File**: `core/enjinnEditor/logs/logWindow.h`

**Purpose**: Console output and debug logs

**Features**:
- Color-coded log levels (info, warning, error)
- Filtering
- Search
- Auto-scroll

### 5. Container Manager Window

**File**: `core/enjinnEditor/containersWindow/containersWindow.h`

**Purpose**: Manage running game containers (hot-reload DLLs)

**Features**:
- List all loaded game modules
- Start/stop containers
- Reload DLLs without restarting

### 6. Asset Manager Window

**File**: `core/enjinnEditor/assetManagerWindow/assetManagerWindow.h`

**Purpose**: File browser for game resources

**Features**:
- File/folder icons (using large FontAwesome glyphs)
- Navigate project directories
- Preview assets
- Drag-drop to containers

### 7. Console Window

**File**: `core/enjinnEditor/enjinnConsoleManager/enjinnConsoleWindow.h`

**Purpose**: Interactive command console

**Features**:
- Execute commands
- History
- Auto-complete

### 8. Shortcuts Editor Window

**File**: `core/enjinnEditor/editShortcuts/editShortcuts.h`

**Purpose**: Customize keyboard shortcuts

**Features**:
- List all shortcuts
- Rebind keys
- Save/load configurations

### 9. OpenGL Errors Window

**File**: `core/enjinnEditor/openglLogs/openglLogs.h`

**Purpose**: OpenGL error tracking

**Features**:
- Display GL errors
- Stack traces
- Error frequency counts

---

## Custom UI Components

EnJinn provides custom ImGui widgets for common UI patterns.

### Colored Buttons

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 259-329)

```cpp
// Red button
bool redButton(const char *label, const ImVec2 &size_arg = {})
{
    return colouredButton(label, {1,0,0,1}, size_arg);
}

// Green button
bool greenButton(const char *label, const ImVec2 &size_arg = {})
{
    return colouredButton(label, {0,1,0,1}, size_arg);
}

// Blue button
bool blueButton(const char *label, const ImVec2 &size_arg = {})
{
    return colouredButton(label, {0,0,1,1}, size_arg);
}

// Custom color button
bool colouredButton(const char *label, glm::vec4 color, const ImVec2 &size_arg)
{
    ::ImGui::PushStyleColor(ImGuiCol_Button, {color.r, color.g, color.b, color.a});
    auto rez = ::ImGui::Button(label, size_arg);
    ::ImGui::PopStyleColor();
    return rez;
}
```

**Usage**:
```cpp
if (enjinn::enjinnImgui::redButton("Delete"))
{
    // Delete action
}
```

### Color Picker with Swatches

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 284-313)

```cpp
// Color picker with preset color swatches
bool ColorEdit4Swatches(const char *label, float col[4], ImGuiColorEditFlags flags = 0)
{
    bool rez = ::ImGui::ColorEdit4(label, col);
    
    ::ImGui::BeginGroup();
    ::ImGui::PushID(label);
    
    if (::ImGui::BeginPopup("picker"))
    {
        // Preset colors: Black, Red, Green, Blue, Yellow, Magenta, Cyan, White
        addColorButton("0", {0,0,0,1}, col); ImGui::SameLine(); 
        addColorButton("1", {1,0,0,1}, col); ImGui::SameLine();
        addColorButton("2", {0,1,0,1}, col); ImGui::SameLine();
        addColorButton("3", {0,0,1,1}, col); ImGui::SameLine();
        addColorButton("4", {1,1,0,1}, col); ImGui::SameLine();
        addColorButton("5", {1,0,1,1}, col); ImGui::SameLine();
        addColorButton("6", {0,1,1,1}, col); ImGui::SameLine();
        addColorButton("7", {1,1,1,1}, col);
        
        ::ImGui::EndPopup();
    }
    
    ::ImGui::PopID();
    ::ImGui::EndGroup();
    
    return rez;
}
```

### Colored Child Frame

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 332-345)

```cpp
// Create a child window with custom background color
bool BeginChildFrameColoured(ImGuiID id, glm::vec4 color, 
                             const ImVec2 &size = {}, 
                             ImGuiWindowFlags extra_flags = 0)
{
    ImGuiContext &g = *::ImGui::GetCurrentContext();
    const ImGuiStyle &style = ::ImGui::GetStyle();
    
    ::ImGui::PushStyleColor(ImGuiCol_ChildBg, {color.x, color.y, color.z, color.w});
    ::ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
    ::ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
    ::ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    
    bool ret = ::ImGui::BeginChild(id, size, true, 
                                   ImGuiWindowFlags_NoMove | 
                                   ImGuiWindowFlags_AlwaysUseWindowPadding | 
                                   extra_flags);
    
    ::ImGui::PopStyleVar(3);
    ::ImGui::PopStyleColor();
    return ret;
}
```

### Error & Warning Symbols

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 348-361)

```cpp
// Red error icon
void addErrorSymbol()
{
    ::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));  // Red
    ::ImGui::Text(ICON_FK_TIMES_CIRCLE " ");
    ::ImGui::PopStyleColor();
}

// Yellow warning icon
void addWarningSymbol()
{
    ::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));  // Yellow
    ::ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE " ");
    ::ImGui::PopStyleColor();
}
```

### Help Marker (Tooltip)

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 364-375)

```cpp
// (?) icon with hover tooltip
void helpMarker(const char *desc)
{
    ::ImGui::TextDisabled("(?)");
    if (::ImGui::IsItemHovered())
    {
        ::ImGui::BeginTooltip();
        ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);  // Wrap text
        ::ImGui::TextUnformatted(desc);
        ::ImGui::PopTextWrapPos();
        ::ImGui::EndTooltip();
    }
}
```

**Usage**:
```cpp
ImGui::Text("Some Setting");
ImGui::SameLine();
enjinn::enjinnImgui::helpMarker("This setting controls XYZ...");
```

### Alignment Helper

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 379-386)

```cpp
// Center or align content horizontally
void alignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle &style = ::ImGui::GetStyle();
    float avail = ::ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ::ImGui::SetCursorPosX(::ImGui::GetCursorPosX() + off);
}
```

**Usage**:
```cpp
alignForWidth(100, 0.5f);  // Center 100px element
ImGui::Button("Centered Button", ImVec2(100, 0));
```

### Memory Size Display

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 392-424)

```cpp
static int sizesType = 0;  // 0=Bytes, 1=KB, 2=MB, 3=GB

// Display memory value in selected unit
void displayMemorySizeValue(size_t value)
{
    switch (sizesType)
    {
    case 0:
        ImGui::Text("%" IM_PRIu64 " (bytes)", value);
        break;
    case 1:
        ImGui::Text("%f (KB)", enjinn::BYTES_TO_KB(value));
        break;
    case 2:
        ImGui::Text("%f (MB)", enjinn::BYTES_TO_MB(value));
        break;
    case 3:
        ImGui::Text("%f (GB)", enjinn::BYTES_TO_GB(value));
        break;
    }
}

// Toggle units (dropdown)
void displayMemorySizeToggle()
{
    ImGui::Combo("Sizes type##enjinn", &sizesType, "Bytes\0KB\0MB\0GB\0");
}
```

### File Selector

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.h` (Lines 120-141)

```cpp
struct FileSelector
{
    FileSelector() {};
    
    FileSelector(std::string title, std::string pwd, std::vector<std::string> typeFilters)
    {
        setInfo(std::move(title), std::move(pwd), std::move(typeFilters));
    }
    
    void setInfo(std::string title, std::string pwd, std::vector<std::string> typeFilters);
    
    char file[260] = {};
    ImGui::FileBrowser fileBrowser;
    
    // Returns true on new file selected
    bool run(int id);
};
```

**Usage**:
```cpp
FileSelector selector("Select Texture", "./resources", {".png", ".jpg"});

if (selector.run(imguiId))
{
    std::cout << "Selected: " << selector.file << '\n';
}
```

---

## Layout System

EnJinn supports saving and loading custom window layouts, similar to Unity and Unreal.

### Save Layout

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 247-263)

```cpp
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
    // ...
}
```

**Saved Location**: `engineResources/engineSaves/layouts/*.ini`

### Load Layout

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 265-286)

```cpp
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
```

### Reset Layout

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 290-296)

```cpp
if (ImGui::MenuItem(ICON_FK_REFRESH " Reset Defaults"))
{
    ImGui::LoadIniSettingsFromDisk("imgui.ini"); 
    logs.log("Layout reset to default");
}
```

**Default INI**: `imgui.ini` (auto-saved by ImGui)

### Docking Configuration

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 152-155)

```cpp
// Main window dockspace
static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);
```

**Global Dockspace**:
```cpp
// File: enjinnImgui.cpp (Line 221)
::ImGui::DockSpaceOverViewport(::ImGui::GetMainViewport());
```

This creates a full-window dockspace that all editor windows can dock to.

---

## Menu System

The main menu bar is defined in the editor and provides access to all engine functions.

### Menu Bar Structure

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 158-336)

```cpp
if (ImGui::BeginMenuBar())
{
    // === EnJinn Menu ===
    if (ImGui::BeginMenu(ICON_FK_COGS " EnJinn"))
    {
        if (ImGui::MenuItem(ICON_FK_REFRESH " Reload dll",
            shortcutManager.getShortcut(RELOAD_DLL_SHORTCUTS), nullptr))
        {
            shouldReloadDll = true;
        }
        ImGui::EndMenu();
    }

    // === Options Menu ===
    if (ImGui::BeginMenu("Options"))
    {
        ImGui::MenuItem(DOCK_MAIN_WINDOW_SHORTCUT,
            shortcutManager.getShortcut(DOCK_MAIN_WINDOW_SHORTCUT), 
            &optionsFlags.hideMainWindow);
        ImGui::EndMenu();
    }

    // === Gameplay Menu ===
    if (ImGui::BeginMenu(ICON_FK_GAMEPAD " Gameplay"))
    {
        if (loadedDll.containerInfo.empty())
        {
            ImGui::TextDisabled("No games available");
        }
        else
        {
            // List all available game containers
            for (auto &c : loadedDll.containerInfo)
            {
                if (ImGui::MenuItem(c.containerName.c_str()))
                {
                    containerManager.createContainer(c.containerName, loadedDll, 
                                                    logs, imguiIDsManager, 
                                                    &consoleWindow, std::string());
                }
            }
        }
        ImGui::EndMenu();
    }

    // === Windows Menu ===
    if (ImGui::BeginMenu(ICON_FK_WINDOW_MAXIMIZE " Windows"))
    {
        ImGui::MenuItem(enjinn::LogWindow::ICON_NAME,
            shortcutManager.getShortcut(LOGS_SHORTCUT), &windowFlags.logsWindow);
        
        ImGui::MenuItem(enjinn::ContainersWindow::ICON_NAME,
            shortcutManager.getShortcut(CONTAINERS_SHORTCUTS), &windowFlags.containerManager);
        
        ImGui::MenuItem(enjinn::ConsoleWindow::ICON_NAME,
            shortcutManager.getShortcut(CONSOLE_WINDOW), &windowFlags.consoleWindow);
        
        ImGui::MenuItem(enjinn::AssetManagerWindow::ICON_NAME,
            shortcutManager.getShortcut(ASSET_MANAGER_WINDOW), &windowFlags.assetManagerWindow);
        
        ImGui::MenuItem(enjinn::OpenglLogsWindow::ICON_NAME,
            shortcutManager.getShortcut(OPENGL_ERRORS_WINDOW), &windowFlags.openglErrorsWindow);
        
        ImGui::MenuItem(enjinn::HierarchyWindow::ICON_NAME,
            shortcutManager.getShortcut(HIERARCHY_WINDOW), &windowFlags.hierarchyWindow);
        
        ImGui::MenuItem(enjinn::InspectorWindow::ICON_NAME,
            shortcutManager.getShortcut(INSPECTOR_WINDOW), &windowFlags.inspectorWindow);
        
        ImGui::MenuItem(enjinn::SceneViewWindow::ICON_NAME,
            shortcutManager.getShortcut(SCENE_VIEW_WINDOW), &windowFlags.sceneViewWindow);
        
        ImGui::EndMenu();
    }

    // === Settings Menu ===
    if (ImGui::BeginMenu(ICON_FK_COG " Settings"))
    {
        ImGui::MenuItem(enjinn::EditShortcutsWindow::ICON_NAME,
            shortcutManager.getShortcut(EDIT_SHORTCUTS), &windowFlags.editShortcutsWindow);
        
        enjinn::enjinnImgui::displayMemorySizeToggle();
        
        ImGui::MenuItem(TRANSPARENT_EDITOR_WINDOW,
            shortcutManager.getShortcut(TRANSPARENT_EDITOR_WINDOW), 
            &windowFlags.transparentWindow);
        
        ImGui::Separator();
        
        // Layouts submenu (see Layout System section)
        if (ImGui::BeginMenu(ICON_FK_COLUMNS " Layouts"))
        {
            // Save/Load/Reset layout options
        }
        
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}
```

### Logo Integration

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 302-333)

```cpp
if (logoTexture.id)
{
    // Fit logo in menu bar height
    float height = ImGui::GetFrameHeight(); 
    float padding = 4.0f;
    float drawHeight = height - padding;
    
    if(drawHeight > 0)
    {
        float aspect = (float)logoTexture.GetSize().x / (float)logoTexture.GetSize().y;
        float width = drawHeight * aspect;

        // Center the image in the window
        float windowWidth = ImGui::GetWindowWidth();
        float cursorX = (windowWidth - width) * 0.5f;
        
        // Only set cursor if it doesn't overlap menu items
        if (cursorX > ImGui::GetCursorPosX())
        {
            ImGui::SetCursorPosX(cursorX);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 0.5f);
            
            // Display logo (UVs flipped to correct vertical orientation)
            ImGui::Image((void*)(intptr_t)logoTexture.id, 
                        { width, drawHeight }, 
                        ImVec2(0, 1), ImVec2(1, 0));
        }
    }
}
```

**Logo File**: `EnJinn/engineResources/LOGO.png`

---

## Shortcuts & Keybindings

EnJinn has a comprehensive shortcut system with customizable keybindings.

### Default Shortcuts

**File**: `core/enjinnEditor/editor/editor.cpp` (Lines 38-49)

```cpp
void enjinn::Editor::init(enjinn::ShortcutManager &shortcutManager, ...)
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
}
```

### Shortcut Table

| Action | Default Shortcut | Description |
|--------|-----------------|-------------|
| **Hide Main Window** | `Ctrl+Alt+D` | Toggle main editor visibility |
| **Logs Window** | `Ctrl+L` | Open/close log window |
| **Container Manager** | `Ctrl+M` | Manage game containers |
| **Reload DLL** | `Ctrl+Alt+R` | Hot-reload game code |
| **Transparent Mode** | `Ctrl+Alt+T` | Make windows transparent |
| **Console** | `Ctrl+C` | Open console window |
| **Asset Manager** | `Ctrl+Alt+A` | Open asset browser |
| **OpenGL Errors** | `Ctrl+Alt+O` | View GL errors |
| **Hierarchy** | *(none)* | Scene hierarchy |
| **Inspector** | *(none)* | Property inspector |  
| **Scene View** | *(none)* | Game view |

### Shortcut System API

```cpp
// File: shortcutApi/shortcutApi.h

class ShortcutManager
{
public:
    void registerShortcut(const char* actionName, const char* defaultBinding, bool* flag);
    const char* getShortcut(const char* actionName);
    void update(Input& input);
};
```

**Editor Window**: `core/enjinnEditor/editShortcuts/editShortcuts.h`

Users can customize all shortcuts through the UI (Settings → Edit Shortcuts).

---

## Memory Management

EnJinn uses a custom allocator for all ImGui memory operations.

### Custom Allocator Integration

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.cpp` (Lines 21-44)

```cpp
void *enjinn::enjinnImgui::imguiCustomAlloc(size_t sz, void *user_data)
{
    enjinn::memory::FreeListAllocator *allocator = 
        (enjinn::memory::FreeListAllocator *)user_data;
    
    ENJINN_DEVELOPMENT_ONLY_ASSERT(allocator, "no allocator for imgui");
    
    return allocator->allocate(sz);
}

void enjinn::enjinnImgui::imguiCustomFree(void *ptr, void *user_data)
{
    enjinn::memory::FreeListAllocator *allocator = 
        (enjinn::memory::FreeListAllocator *)user_data;
    
    ENJINN_DEVELOPMENT_ONLY_ASSERT(allocator, "no allocator for imgui");
    
    allocator->free(ptr);
}

void enjinn::enjinnImgui::setImguiAllocator(enjinn::memory::FreeListAllocator &allocator)
{
    ::ImGui::SetAllocatorFunctions(imguiCustomAlloc, imguiCustomFree, &allocator);
}
```

**Benefits**:
- Centralized memory tracking
- Custom allocation strategies
- Debugging support
- Memory profiling

### ImGui ID Management

To avoid ID conflicts, EnJinn provides an ID manager:

**File**: `core/sharedRuntime/enjinnImgui/enjinnImgui.h` (Lines 70-84)

```cpp
namespace EditorImguiIds
{
    enum
    {
        idsCount = 4000  // Reserved for editor
    };
}

struct ImGuiIdsManager
{
    int counter = EditorImguiIds::idsCount + 1;
    
    // Returns the first id. (count) ids will be reserved.
    // If you want 5 ids and the function returns 10, 
    // then ids 10, 11, 12, 13, 14 will be reserved.
    int getImguiIds(unsigned int count = 1)
    {
        if (count == 0) { return 0; }
        auto c = counter;
        counter += count;
        return c;
    }
};
```

**Usage**:
```cpp
int myId = imguiIDManager.getImguiIds(5);  // Reserve 5 IDs

ImGui::PushID(myId);
// Use IDs myId, myId+1, myId+2, myId+3, myId+4
ImGui::PopID();
```

---

## Code Examples & Snippets

### Basic Window Creation

```cpp
void MyWindow::update(bool &open)
{
    if (!open) return;
    
    ImGui::PushID(windowId);
    
    if (ImGui::Begin(ICON_FK_INFO " My Window", &open))
    {
        ImGui::Text("Hello, World!");
        
        if (ImGui::Button("Click Me"))
        {
            std::cout << "Button clicked!\n";
        }
    }
    
    ImGui::End();
    ImGui::PopID();
}
```

### Hierarchical Tree

```cpp
void drawNode(Node* node)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | 
                              ImGuiTreeNodeFlags_OpenOnDoubleClick;
    
    if (node == selectedNode)
        flags |= ImGuiTreeNodeFlags_Selected;
    
    if (node->children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;
    
    bool nodeOpen = ImGui::TreeNodeEx(node, flags, "%s %s", 
                                     ICON_FK_CUBE, node->name.c_str());
    
    if (ImGui::IsItemClicked())
        selectedNode = node;
    
    if (nodeOpen)
    {
        for (auto* child : node->children)
        {
            drawNode(child);  // Recursive!
        }
        ImGui::TreePop();
    }
}
```

### Tab Bar

```cpp
if (ImGui::BeginTabBar("MyTabBar"))
{
    if (ImGui::BeginTab Item(ICON_FK_HOME " Home"))
    {
        ImGui::Text("Home tab content");
        ImGui::EndTabItem();
    }
    
    if (ImGui::BeginTabItem(ICON_FK_COG " Settings"))
    {
        ImGui::Text("Settings tab content");
        ImGui::EndTabItem();
    }
    
    ImGui::EndTabBar();
}
```

### Property Editor (Inspector Style)

```cpp
void displayNodeProperties(Node* node)
{
    if (!node) return;
    
    ImGui::Text(ICON_FK_CUBE " Node: %s", node->name.c_str());
    ImGui::Separator();
    
    // Transform
    if (ImGui::CollapsingHeader(ICON_FK_ARROWS " Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Position", &node->localPosition.x, 0.1f);
        ImGui::DragFloat3("Rotation", &node->localRotation.x, 1.0f);
        ImGui::DragFloat3("Scale", &node->localScale.x, 0.01f);
    }
    
    // Components  
    if (ImGui::CollapsingHeader(ICON_FK_PUZZLE_PIECE " Components"))
    {
        for (auto* component : node->components)
        {
            if (ImGui::TreeNode(component->getTypeName()))
            {
                component->onInspector();  // Custom component UI
                ImGui::TreePop();
            }
        }
    }
}
```

### Drag & Drop

```cpp
// Drag source
if (ImGui::BeginDragDropSource())
{
    ImGui::SetDragDropPayload("NODE_PTR", &node, sizeof(Node*));
    ImGui::Text("Dragging: %s", node->name.c_str());
    ImGui::EndDragDropSource();
}

// Drop target
if (ImGui::BeginDragDropTarget())
{
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_PTR"))
    {
        Node* droppedNode = *(Node**)payload->Data;
        // Reparent droppedNode to this node
        node->addChild(droppedNode);
    }
    ImGui::EndDragDropTarget();
}
```

### Table Layout

```cpp
if (ImGui::BeginTable("MyTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
{
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();
    
    for (auto& property : properties)
    {
        ImGui::TableNextRow();
        
        ImGui::TableNextColumn();
        ImGui::Text("%s", property.name.c_str());
        
        ImGui::TableNextColumn();
        ImGui::Text("%s", property.type.c_str());
        
        ImGui::TableNextColumn();
        ImGui::Text("%s", property.value.c_str());
    }
    
    ImGui::EndTable();
}
```

---

## Additional Resources

### Documentation Links

- **ImGui Manual**: Built-in `imgui_demo.cpp` (comprehensive examples)
- **ImGui GitHub**: https://github.com/ocornut/imgui
- **FontAwesome 4 Icons**: https://fontawesome.com/v4/icons/
- **IconFontCppHeaders**: https://github.com/juliettef/IconFontCppHeaders
- **ImGui Fonts Guide**: https://pixtur.github.io/mkdocs-for-imgui/site/FONTS/

### Key Files Reference

| File | Purpose |
|------|---------|
| `enjinnImgui/enjinnImgui.h` | ImGui wrapper API |
| `enjinnImgui/enjinnImgui.cpp` | ImGui initialization & theme |
| `editor/editor.h` | Main editor structure |
| `editor/editor.cpp` | Menu bar & window management |
| `editor/hierarchyWindow.*` | Scene hierarchy tree |
| `editor/inspectorWindow.*` | Property inspector |
| `editor/sceneViewWindow.*` | Game view |
| `assetManagerWindow/*` | File browser |
| `IconsForkAwesome.h` | Icon definitions |

### File Paths

- **Fonts**: `EnJinn/resources/*.ttf`
- **Logo**: `EnJinn/engineResources/LOGO.png`
- **Layouts**: `EnJinn/engineResources/engineSaves/layouts/*.ini`
- **Options**: `EnJinn/engineResources/engineSaves/options.bin`
- **Window State**: `EnJinn/engineResources/engineSaves/window.bin`

---

## Summary

The EnJinn UI system is a professionally designed, highly customizable editor interface built on Dear ImGui. Key strengths include:

✅ **Modern Design** - Unity-inspired dark theme with vibrant blue accents  
✅ **Docking Support** - Full window docking and multi-viewport  
✅ **Icon Integration** - FontAwesome 4 icons for visual clarity  
✅ **Extensible** - Custom widgets and components  
✅ **Persistent Layouts** - Save/load window arrangements  
✅ **Performance** - Custom memory allocator integration  
✅ **Developer Friendly** - Clean API and extensive documentation  

The UI is ready for your future improvements over the next 6 months. All components are well-structured, documented, and follow modern game engine practices!

---

*Last Updated: 2026-01-20*  
*Engine Version: EnJinn Development Build*
