#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#include <enjinnImgui/enjinnImgui.h>
#include "IconsFontAwesome6.h"
#include <enjinnAllocator/freeListAllocator.h>
#include <logs/assert.h>
#include <compilerIntrinsics.h>
#include <stringManipulation/stringManipulation.h>
#include "imgui_internal.h"
#include <filesystem>


//todo macro to remove imgui impl


#if !(ENJINN_SHOULD_REMOVE_IMGUI)


void *enjinn::enjinnImgui::imguiCustomAlloc(size_t sz, void *user_data)
{
    enjinn::memory::FreeListAllocator *allocator = (enjinn::memory::FreeListAllocator *)user_data;
    ENJINN_DEVELOPMENT_ONLY_ASSERT(allocator, "no allocator for imgui");


    return allocator->allocate(sz);
}


void enjinn::enjinnImgui::imguiCustomFree(void *ptr, void *user_data)
{
    enjinn::memory::FreeListAllocator *allocator = (enjinn::memory::FreeListAllocator *)user_data;
    ENJINN_DEVELOPMENT_ONLY_ASSERT(allocator, "no allocator for imgui");


    allocator->free(ptr);
}


void enjinn::enjinnImgui::setImguiAllocator(enjinn::memory::FreeListAllocator &allocator)
{
    ::ImGui::SetAllocatorFunctions(imguiCustomAlloc, imguiCustomFree, &allocator);
}



void enjinn::enjinnImgui::initImgui(EnJinnContext &enjinnContext)
{
    setImguiAllocator(enjinnContext.imguiAllocator);

    HWND hwnd = glfwGetWin32Window(enjinnContext.wind);
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));


    auto context = ::ImGui::CreateContext();
    //ImGui::StyleColorsDark();
    //imguiThemes::embraceTheDarkness();
    
    // Modern Flat Theme - Unity Inspired
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.TabRounding = 4.0f;
    
    // Better padding and spacing
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;


    ImVec4* colors = style.Colors;
    // Unity-inspired dark theme with blue accent
    colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.95f);
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);


    ImGuiIO &io = ::ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io.FontGlobalScale = 1.0f; // Proper scaling for 2K displays


    //ImGuiStyle &style = ::ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        //style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.f;
        style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
    }




    ImGui_ImplGlfw_InitForOpenGL(enjinnContext.wind, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    enjinnContext.ImGuiContext = context;




    //https://pixtur.github.io/mkdocs-for-imgui/site/FONTS/
    //https://github.com/juliettef/IconFontCppHeaders
    //https://fontawesome.com/icons (FontAwesome 6)
    
    // NOTE: FontAwesome 6 fonts must be downloaded separately
    // Download from: https://fontawesome.com/download (Free version)
    // Place fa-solid-900.ttf in EnJinn/resources/
    
 io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "Poppins-Regular.ttf", 15);// Increased from 12 for better readability


    //ImFontGlyphRangesBuilder builder;
    //builder.AddText("Hello world");                        // Add a string (here "Hello world" contains 7 unique characters)
    //builder.AddChar(0x7262);                               // Add a specific character
    //builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // Add one of the default ranges
    //builder.BuildRanges(&ranges);



    ImFontConfig config;
config.MergeMode = true;
    config.PixelSnapH = true;  // Better alignment
    // config.GlyphMinAdvanceX = 16.0f; // Commented out - can cause rendering issues

    static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    
    // Try to load FA6, fallback to FA4 if not available
    bool fa6Loaded = false;
    if (std::filesystem::exists(ENJINN_RESOURCES_PATH "fa-solid-900.ttf"))
    {
        io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "fa-solid-900.ttf", 14.0f,  &config, icon_ranges);
        fa6Loaded = true;
    }
    else if (std::filesystem::exists(ENJINN_RESOURCES_PATH "fontawesome-webfont.ttf"))
    {
        // Fallback to FA4 if FA6 not found
        io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "fontawesome-webfont.ttf", 14.0f, &config, icon_ranges);
    }


    {
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddChar(0xf15b);//ICON_FA_FILE
        builder.AddChar(0xf07b);//ICON_FA_FOLDER
        builder.BuildRanges(&ranges);


        // Large icons for asset manager (100px)
        const char* largeIconFont = fa6Loaded ? 
            ENJINN_RESOURCES_PATH "fa-solid-900.ttf" :
            ENJINN_RESOURCES_PATH "fontawesome-webfont.ttf";
            
        if (std::filesystem::exists(largeIconFont))
        {
            io.Fonts->AddFontFromFileTTF(largeIconFont,
                100.f/ io.FontGlobalScale, 0, ranges.Data);
        }

        // High-res text for Asset Manager scaling (Index 2)
        io.Fonts->AddFontFromFileTTF(ENJINN_RESOURCES_PATH "Poppins-Regular.ttf", 64.0f);
    }
    io.Fonts->Build();



}


void enjinn::enjinnImgui::setImguiContext(EnJinnContext enjinnContext)
{
    ::ImGui::SetCurrentContext(enjinnContext.ImGuiContext);
}


void enjinn::enjinnImgui::imguiStartFrame(EnJinnContext enjinnContext)
{
    setImguiContext(enjinnContext);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ::ImGui::NewFrame();
    ::ImGui::DockSpaceOverViewport(::ImGui::GetMainViewport());
}



void enjinn::enjinnImgui::imguiEndFrame(EnJinnContext enjinnContext)
{
    setImguiContext(enjinnContext);
    ::ImGui::Render();
    int display_w = 0, display_h = 0;
    glfwGetFramebufferSize(enjinnContext.wind, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());


    ImGuiIO &io = ::ImGui::GetIO();


    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        //GLFWwindow *backup_current_context = glfwGetCurrentContext();
        //ImGui::UpdatePlatformWindows();
        //ImGui::RenderPlatformWindowsDefault();
        //glfwMakeContextCurrent(backup_current_context);


        ::ImGui::UpdatePlatformWindows();
        ::ImGui::RenderPlatformWindowsDefault();
        enjinnContext.glfwMakeContextCurrentPtr(enjinnContext.wind); //idea create a class with some functions


    }
}


bool enjinn::enjinnImgui::redButton(const char *label, const ImVec2 &size_arg)
{
    return colouredButton(label, {1,0,0,1}, size_arg);
}


bool enjinn::enjinnImgui::greenButton(const char *label, const ImVec2 &size_arg)
{
    return colouredButton(label, {0,1,0,1}, size_arg);
}



void addColorButton(const char *id, const ImVec4 &col, float outCol[4])
{
    if (ImGui::ColorButton(id, col))
    {
        outCol[0] = col.x;
        outCol[1] = col.y;
        outCol[2] = col.z;
        outCol[3] = col.w;
    }
}


bool enjinn::enjinnImgui::ColorEdit4Swatches(const char *label, float col[4], ImGuiColorEditFlags flags)
{
    bool rez = ::ImGui::ColorEdit4(label, col);


    ::ImGui::BeginGroup();
    ::ImGui::PushID(label);


    if (::ImGui::BeginPopup("picker"))
    {
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


bool enjinn::enjinnImgui::blueButton(const char *label, const ImVec2 &size_arg)
{
    return colouredButton(label, {0,0,1,1}, size_arg);
}


bool enjinn::enjinnImgui::colouredButton(const char *label, glm::vec4 color,
    const ImVec2 &size_arg)
{
    ::ImGui::PushStyleColor(ImGuiCol_Button, {color.r, color.g, color.b, color.a});
    auto rez = ::ImGui::Button(label, size_arg);
    ::ImGui::PopStyleColor();
    return rez;
}


bool enjinn::enjinnImgui::BeginChildFrameColoured(ImGuiID id,
    glm::vec4 color, const ImVec2 &size, ImGuiWindowFlags extra_flags)
{
    ImGuiContext &g = *::ImGui::GetCurrentContext();
    const ImGuiStyle &style = ::ImGui::GetStyle();
    ::ImGui::PushStyleColor(ImGuiCol_ChildBg, {color.x,color.y,color.z,color.w});
    ::ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
    ::ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
    ::ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    bool ret = ::ImGui::BeginChild(id, size, true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
    ::ImGui::PopStyleVar(3);
    ::ImGui::PopStyleColor();
    return ret;
}


void enjinn::enjinnImgui::addErrorSymbol()
{
    ::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
    ::ImGui::Text(ICON_FK_TIMES_CIRCLE " ");
    ::ImGui::PopStyleColor();
}


void enjinn::enjinnImgui::addWarningSymbol()
{
    ::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
    ::ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE " ");
    ::ImGui::PopStyleColor();
}


void enjinn::enjinnImgui::helpMarker(const char *desc)
{
    ::ImGui::TextDisabled("(?)");
    if (::ImGui::IsItemHovered())
    {
        ::ImGui::BeginTooltip();
        ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
        ::ImGui::TextUnformatted(desc);
        ::ImGui::PopTextWrapPos();
        ::ImGui::EndTooltip();
    }
}


//https://github.com/ocornut/imgui/discussions/3862
void enjinn::enjinnImgui::alignForWidth(float width, float alignment)
{
    ImGuiStyle &style = ::ImGui::GetStyle();
    float avail = ::ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ::ImGui::SetCursorPosX(::ImGui::GetCursorPosX() + off);
}


static int sizesType = 0;


void enjinn::enjinnImgui::displayMemorySizeValue(size_t value)
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
    default:
        ENJINN_UNREACHABLE();
    }


    //ImGui::Text("%" IM_PRIu64 " (bytes)", value); 


}


void enjinn::enjinnImgui::displayMemorySizeToggle()
{
    ImGui::Combo("Sizes type##enjinn", &sizesType, "Bytes\0KB\0MB\0GB\0");
}


void enjinn::enjinnImgui::removeFocusToCurrentWindow()
{
    auto *g = ::ImGui::GetCurrentContext();
    if (g)
    {
        g->NavWindow = 0;
    }
}


void enjinn::enjinnImgui::FileSelector::setInfo(std::string title, std::string pwd, std::vector<std::string> typeFilters)
{
    fileBrowser.SetTitle(std::move(title));
    fileBrowser.SetPwd(std::move(pwd));
    
    if (!typeFilters.empty())
    {
        fileBrowser.SetTypeFilters(std::move(typeFilters));
    }
}


bool enjinn::enjinnImgui::FileSelector::run(int id)
{
    ImGui::PushID(id);
    
    bool r = ImGui::InputText(fileBrowser.getTitle().c_str(), this->file, sizeof(file), ImGuiInputTextFlags_EnterReturnsTrue);
    
    ImGui::SameLine();
    if (ImGui::Button("Select file"))
    {
        fileBrowser.Open();
    }
    
    fileBrowser.Display();
    
    if (fileBrowser.HasSelected())
    {
        enjinn::strlcpy(file, fileBrowser.GetSelected().string(), sizeof(file));
    
        fileBrowser.ClearSelected();
        fileBrowser.Close();


        r = true;
    }
    
    ImGui::PopID();
    
    return r;
}



#else


void enjinn::enjinnImgui::alignForWidth(float width, float alignment) {};
void enjinn::enjinnImgui::helpMarker(const char *desc) {};
void enjinn::enjinnImgui::addWarningSymbol() {};
void enjinn::enjinnImgui::addErrorSymbol() {};
void enjinn::enjinnImgui::imguiEndFrame(EnJinnContext enjinnContext) {};
void enjinn::enjinnImgui::imguiStartFrame(EnJinnContext enjinnContext) {};
void enjinn::enjinnImgui::setImguiContext(EnJinnContext enjinnContext) {};
void enjinn::enjinnImgui::initImgui(EnJinnContext &enjinnContext) {};
void enjinn::enjinnImgui::setImguiAllocator(enjinn::memory::FreeListAllocator &allocator) {};
bool enjinn::enjinnImgui::FileSelector::run(int id) { return false; };
void enjinn::enjinnImgui::FileSelector::setInfo(std::string title, std::string pwd, std::vector<std::string> typeFilters) {};


#endif

