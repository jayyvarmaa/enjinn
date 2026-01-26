#pragma once
//////////////////////////////////////////
// propertyDrawer.h
// EnJinn Engine - Property Drawer System
// Automatic UI generation for component properties
//////////////////////////////////////////

#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <imgui.h>

namespace enjinn
{
namespace PropertyDrawer
{
    //========================================
    // Type-Specific Property Drawers
    //========================================
    
    // Integers
    inline bool DrawInt(const char* label, int* value, int step = 1)
    {
        return ImGui::DragInt(label, value, float(step));
    }
    
    inline bool DrawIntSlider(const char* label, int* value, int min, int max)
    {
        return ImGui::SliderInt(label, value, min, max);
    }
    
    // Floats
    inline bool DrawFloat(const char* label, float* value, float speed = 0.1f)
    {
        return ImGui::DragFloat(label, value, speed);
    }
    
    inline bool DrawFloatSlider(const char* label, float* value, float min, float max)
    {
        return ImGui::SliderFloat(label, value, min, max);
    }
    
    inline bool DrawFloatRange(const char* label, float* value, float min, float max, float speed = 0.1f)
    {
        return ImGui::DragFloat(label, value, speed, min, max);
    }
    
    // Booleans
    inline bool DrawBool(const char* label, bool* value)
    {
        return ImGui::Checkbox(label, value);
    }
    
    // Strings
    inline bool DrawString(const char* label, char* buffer, size_t bufferSize)
    {
        return ImGui::InputText(label, buffer, bufferSize);
    }
    
    inline bool DrawStringMultiline(const char* label, char* buffer, size_t bufferSize, const ImVec2& size = ImVec2(0, 0))
    {
        return ImGui::InputTextMultiline(label, buffer, bufferSize, size);
    }
    
    // Vectors
    inline bool DrawVec2(const char* label, glm::vec2* value, float speed = 0.1f)
    {
        return ImGui::DragFloat2(label, &value->x, speed);
    }
    
    inline bool DrawVec3(const char* label, glm::vec3* value, float speed = 0.1f)
    {
        return ImGui::DragFloat3(label, &value->x, speed);
    }
    
    inline bool DrawVec4(const char* label, glm::vec4* value, float speed = 0.1f)
    {
        return ImGui::DragFloat4(label, &value->x, speed);
    }
    
    // Colors
    inline bool DrawColor3(const char* label, glm::vec3* value)
    {
        return ImGui::ColorEdit3(label, &value->x);
    }
    
    inline bool DrawColor4(const char* label, glm::vec4* value)
    {
        return ImGui::ColorEdit4(label, &value->x);
    }
    
    inline bool DrawColor4Picker(const char* label, glm::vec4* value)
    {
        return ImGui::ColorPicker4(label, &value->x);
    }
    
    // Angles & Rotation
    inline bool DrawAngle(const char* label, float* radians, float speed = 0.01f)
    {
        float degrees = glm::degrees(*radians);
        if (ImGui::DragFloat(label, &degrees, speed))
        {
            *radians = glm::radians(degrees);
            return true;
        }
        return false;
    }
    
    inline bool DrawRotation(const char* label, glm::vec3* eulerRadians, float speed = 0.01f)
    {
        glm::vec3 degrees = glm::degrees(*eulerRadians);
        if (ImGui::DragFloat3(label, &degrees.x, speed))
        {
            *eulerRadians = glm::radians(degrees);
            return true;
        }
        return false;
    }
    
    //========================================
    // Advanced Property Drawers
    //========================================
    
    // Enum (requires manual mapping)
    template<typename EnumType>
    inline bool DrawEnum(const char* label, EnumType* value, const char* const* names, int count)
    {
        int current = static_cast<int>(*value);
        if (ImGui::Combo(label, &current, names, count))
        {
            *value = static_cast<EnumType>(current);
            return true;
        }
        return false;
    }
    
    // Asset Reference (texture, model, etc.)
    inline bool DrawAssetRef(const char* label, std::string& assetPath, const char* filter = "")
    {
        char buffer[256];
        strncpy(buffer, assetPath.c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        bool changed = false;
        if (ImGui::InputText(label, buffer, sizeof(buffer)))
        {
            assetPath = buffer;
            changed = true;
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Browse..."))
        {
            // TODO: Open file browser
            // This would integrate with the AssetManagerWindow
        }
        
        return changed;
    }
    
    //========================================
    // Property Group Helper
    //========================================
    
    class PropertyGroup
    {
    public:
        PropertyGroup(const char* name, bool defaultOpen = true)
            : name(name)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
            if (!defaultOpen)
                flags = ImGuiTreeNodeFlags_None;
                
            isOpen = ImGui::CollapsingHeader(name, flags);
        }
        
        ~PropertyGroup()
        {
            // Nothing to clean up
        }
        
        operator bool() const { return isOpen; }
        bool IsOpen() const { return isOpen; }
        
    private:
        const char* name;
        bool isOpen;
    };
    
    //========================================
    // Property Decorators
    //========================================
    
    // Read-only display
    inline void DisplayInt(const char* label, int value)
    {
        ImGui::Text("%s: %d", label, value);
    }
    
    inline void DisplayFloat(const char* label, float value, unsigned int precision = 2)
    {
        ImGui::Text("%s: %.*f", label, precision, value);
    }
    
    inline void DisplayVec3(const char* label, const glm::vec3& value)
    {
        ImGui::Text("%s: (%.2f, %.2f, %.2f)", label, value.x, value.y, value.z);
    }
    
    inline void DisplayString(const char* label, const char* value)
    {
        ImGui::Text("%s: %s", label, value);
    }
    
    // Tooltip helper
    inline void AddTooltip(const char* text)
    {
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("%s", text);
        }
    }
    
    // Help marker (? icon with tooltip)
    inline void HelpMarker(const char* desc)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        AddTooltip(desc);
    }
    
    //========================================
    // Combined Property + Tooltip
    //========================================
    
    inline bool DrawFloatWithTooltip(const char* label, float* value, const char* tooltip, float speed = 0.1f)
    {
        bool changed = DrawFloat(label, value, speed);
        HelpMarker(tooltip);
        return changed;
    }
    
    inline bool DrawBoolWithTooltip(const char* label, bool* value, const char* tooltip)
    {
        bool changed = DrawBool(label, value);
        HelpMarker(tooltip);
        return changed;
    }
    
    inline bool DrawVec3WithTooltip(const char* label, glm::vec3* value, const char* tooltip, float speed = 0.1f)
    {
        bool changed = DrawVec3(label, value, speed);
        HelpMarker(tooltip);
        return changed;
    }
    
} // namespace PropertyDrawer
} // namespace enjinn
