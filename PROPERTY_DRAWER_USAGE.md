# Property Drawer System - Usage Guide

## Overview

The Property Drawer system **eliminates boilerplate** when creating component UIs. Instead of writing manual ImGui calls for every property, use simple inline functions.

## Before vs After

### Before (Manual ImGui - Lots of Boilerplate)

```cpp
struct LightComponent : public Component
{
    glm::vec3 color = {1, 1, 1};
    float intensity = 1.0f;
    bool castShadows = true;
    float shadowBias = 0.001f;
    
    void onInspector() override
    {
        ImGui::Text("Light Component");
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit3("Color", &color.x);
            
            ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 10.0f);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Light brightness");
            
            ImGui::Checkbox("Cast Shadows", &castShadows);
            
            if (castShadows)
            {
                ImGui::DragFloat("Shadow Bias", &shadowBias, 0.0001f, 0.0f, 0.01f);
            }
        }
    }
};
```

**Lines of UI code**: ~20 lines for 4 properties

### After (Property Drawers - Clean & Concise)

```cpp
#include <propertyDrawer/propertyDrawer.h>

struct LightComponent : public Component
{
    glm::vec3 color = {1, 1, 1};
    float intensity = 1.0f;
    bool castShadows = true;
    float shadowBias = 0.001f;
    
    void onInspector() override
    {
        using namespace enjinn::PropertyDrawer;
        
        if (PropertyGroup("Properties"))
        {
            DrawColor3("Color", &color);
            DrawFloatRange("Intensity", &intensity, 0.0f, 10.0f);
            HelpMarker("Light brightness");
            
            DrawBool("Cast Shadows", &castShadows);
            
            if (castShadows)
            {
                DrawFloatRange("Shadow Bias", &shadowBias, 0.0f, 0.01f, 0.0001f);
            }
        }
    }
};
```

**Lines of UI code**: ~10 lines for 4 properties  
**Reduction**: **50% less code!**

---

## Available Drawers

### Basic Types

```cpp
using namespace enjinn::PropertyDrawer;

// Integers
DrawInt("Health", &health);
DrawIntSlider("Level", &level, 1, 100);

// Floats
DrawFloat("Speed", &speed);
DrawFloatSlider("Volume", &volume, 0.0f, 1.0f);
DrawFloatRange("Damage", &damage, 1.0f, 100.0f);

// Booleans
DrawBool("Enabled", &enabled);

// Strings
char name[64] = "Player";
DrawString("Name", name, sizeof(name));
DrawStringMultiline("Description", desc, sizeof(desc));
```

### Vectors & Colors

```cpp
glm::vec2 position2D;
glm::vec3 position3D;
glm::vec4 quad;

DrawVec2("Position 2D", &position2D);
DrawVec3("Position 3D", &position3D);
DrawVec4("Quad", &quad);

// Colors
glm::vec3 color3;
glm::vec4 color4;

DrawColor3("RGB Color", &color3);
DrawColor4("RGBA Color", &color4);
DrawColor4Picker("Color Picker", &color4);  // Full picker
```

### Angles & Rotations

```cpp
float angleRad = 0.0f;
glm::vec3 rotationRad = {0, 0, 0};

// Automatically converts to degrees for display
DrawAngle("Rotation Angle", &angleRad);
DrawRotation("Euler Rotation", &rotationRad);
```

### Enums

```cpp
enum class LightType { Point, Directional, Spot };

LightType type = LightType::Point;
const char* typeNames[] = {"Point", "Directional", "Spot"};

DrawEnum("Light Type", &type, typeNames, 3);
```

---

## Property Groups

Organize properties into collapsible sections:

```cpp
if (PropertyGroup("Transform"))
{
    DrawVec3("Position", &position);
    DrawVec3("Rotation", &rotation);
    DrawVec3("Scale", &scale);
}

if (PropertyGroup("Rendering", false))  // Collapsed by default
{
    DrawColor4("Tint", &tint);
    DrawFloatRange("Opacity", &opacity, 0.0f, 1.0f);
}
```

---

## Tooltips & Help

```cpp
// Method 1: Combined function
DrawFloatWithTooltip("Speed", &speed, "Movement speed in units/second");

// Method 2: Separate tooltip
DrawFloat("Speed", &speed);
HelpMarker("Movement speed in units/second");

// Method 3: Manual tooltip
DrawFloat("Speed", &speed);
PropertyDrawer::AddTooltip("Movement speed in units/second");
```

---

## Read-Only Properties

Display values without editing:

```cpp
DisplayInt("Triangle Count", triangleCount);
DisplayFloat("FPS", fps);
DisplayVec3("World Position", worldPos);
DisplayString("Status", "Active");
```

---

## Real-World Example: Transform Component

```cpp
struct TransformComponent : public Component
{
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};  // Radians
    glm::vec3 scale = {1, 1, 1};
    
    void onInspector() override
    {
        using namespace enjinn::PropertyDrawer;
        
        if (PropertyGroup("Transform"))
        {
            if (DrawVec3("Position", &position))
            {
                // Property changed - update transform
                updateMatrix();
            }
            
            DrawRotation("Rotation", &rotation);
            DrawVec3("Scale", &scale, 0.01f);
        }
    }
};
```

---

## Real-World Example: Camera Component

```cpp
struct CameraComponent : public Component
{
    enum ProjectionType { Perspective, Orthographic };
    
    ProjectionType projection = Perspective;
    float fov = 60.0f;  // Degrees
    float nearClip = 0.1f;
    float farClip = 1000.0f;
    glm::vec3 clearColor = {0.2f, 0.3f, 0.4f};
    
    void onInspector() override
    {
        using namespace enjinn::PropertyDrawer;
        
        if (PropertyGroup("Camera"))
        {
            const char* projNames[] = {"Perspective", "Orthographic"};
            DrawEnum("Projection", &projection, projNames, 2);
            
            if (projection == Perspective)
            {
                DrawFloatRange("FOV", &fov, 10.0f, 120.0f);
                HelpMarker("Field of view in degrees");
            }
            
            DrawFloatWithTooltip("Near Clip", &nearClip, "Closest visible distance", 0.01f);
            DrawFloatWithTooltip("Far Clip", &farClip, "Farthest visible distance");
            
            DrawColor3("Clear Color", &clearColor);
        }
    }
};
```

---

## Best Practices

### 1. Use `PropertyGroup` for Organization

```cpp
// Good
if (PropertyGroup("Physics"))
{
    DrawFloat("Mass", &mass);
    DrawFloat("Friction", &friction);
}

if (PropertyGroup("Rendering"))
{
    DrawColor4("Color", &color);
}

// Bad - Everything flat
DrawFloat("Mass", &mass);
DrawFloat("Friction", &friction);
DrawColor4("Color", &color);
```

### 2. Add Tooltips for Complex Properties

```cpp
// Good
DrawFloatRange("Damping", &damping, 0.0f, 1.0f);
HelpMarker("How quickly velocity decreases. 0 = no damping, 1 = full damping");

// Acceptable for obvious properties
DrawBool("Enabled", &enabled);
```

### 3. Use Appropriate Drawers

```cpp
// Good - Use range when there's a logical min/max
DrawFloatRange("Volume", &volume, 0.0f, 1.0f);

// Bad - Unconstrained where it should be constrained
DrawFloat("Volume", &volume);  // User could set negative volume!
```

### 4. Handle Property Changes

```cpp
if (DrawVec3("Position", &position))
{
    // React to change - update transforms, mark dirty, etc.
    onPositionChanged();
}
```

---

## Integration with Component System

### Step 1: Include in Component Header

```cpp
// Component.h
#pragma once

namespace enjinn
{
    struct Component
    {
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onInspector() {}  // Add this!
        
        virtual const char* getTypeName() const { return "Component"; }
    };
}
```

### Step 2: Inspector Window Calls `onInspector()`

```cpp
// inspectorWindow.cpp
void InspectorWindow::update(Node* selectedNode, bool &open)
{
    if (!selectedNode) return;
    
    for (auto* component : selectedNode->components)
    {
        if (ImGui::TreeNode(component->getTypeName()))
        {
            component->onInspector();  // Draw UI automatically!
            ImGui::TreePop();
        }
    }
}
```

---

## Benefits Summary

✅ **50-80% less UI code**  
✅ **Consistent look & feel**  
✅ **Easy to add new properties** (one line!)  
✅ **Built-in tooltips & helpers**  
✅ **Type-safe** (compile-time checks)  
✅ **Hot-reload friendly** (just add properties, UI updates automatically)  

---

## Next: Advanced Reflection

Phase 3 will add **automatic reflection** so you don't even need to write `Drew*` calls!

```cpp
// Future: Phase 3 Enhanced Reflection
struct LightComponent : public Component
{
    ENJINN_PROPERTY(Color3, color, = {1,1,1})
    ENJINN_PROPERTY_FLOAT_RANGE(intensity, 0.0f, 10.0f, = 1.0f)
    ENJ INN_PROPERTY(Bool, castShadows, = true)
    
    // onInspector() generated automatically! 🎉
};
```

But for now, the basic property drawers give you massive productivity gains!
