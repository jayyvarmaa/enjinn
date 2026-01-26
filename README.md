# EnJinn – A Modular 2D/3D Game Engine

## Project Overview

**EnJinn** is a custom-built, modular game engine written in **C++17** using **OpenGL** for rendering. The engine is designed to provide a complete development environment for creating 2D and 3D games with real-time visual editing, physics simulation, and hot-reload capabilities.

This project aims to explore core game engine architecture concepts including rendering pipelines, entity-component systems, memory management, and editor tooling.

---

## Problem Statement

Modern game development requires sophisticated tools that handle rendering, physics, input management, and scene organization. Commercial engines like Unity and Unreal are powerful but opaque—developers don't understand what happens "under the hood."

**EnJinn** addresses this by building a game engine from scratch, providing:
- Deep understanding of graphics programming (OpenGL, shaders)
- Hands-on experience with game architecture patterns
- Custom memory management and optimization techniques
- Real-time editor development using Dear ImGui

---

## Proposed Features

### 🎮 Core Engine
| Feature | Description |
|---------|-------------|
| **Container System** | Hot-reloadable DLL-based game modules for rapid iteration |
| **Scene Graph** | Node/Component architecture for hierarchical game objects |
| **2D Renderer** | Custom `gl2d` library for sprite rendering, animations, and post-processing |
| **3D Renderer** | Custom `gl3d` library for 3D mesh rendering and lighting |
| **Physics Engine** | Box2D integration for realistic 2D physics simulation |
| **Input System** | Unified input handling for keyboard, mouse, and controllers |
| **Asset Manager** | Centralized texture and resource loading/caching |

### 🛠️ Editor Tools
| Tool | Purpose |
|------|---------|
| **Hierarchy Window** | Visual tree-view of all scene nodes |
| **Inspector Window** | Property editing for selected nodes/components |
| **Scene View** | Real-time game preview with editor overlays |
| **Console Window** | Runtime logging and debugging output |
| **Asset Manager** | Browse and manage game resources |

### 🧠 Technical Systems
- **Custom Memory Allocators** – FreeList allocator for controlled memory management
- **Serialization** – SafeSave system for persistent game data
- **Shortcut System** – Configurable keyboard shortcuts
- **Post-Processing** – Shader-based visual effects (blur, color grading)
- **Hot Reload** – Modify gameplay code without restarting the editor

---

## Technology Stack

| Category | Technology |
|----------|------------|
| Language | C++17 |
| Build System | CMake |
| Graphics API | OpenGL 3.3+ (via GLAD) |
| Windowing | GLFW 3.3 |
| UI Framework | Dear ImGui (Docking Branch) |
| Math Library | GLM |
| Physics | Box2D 2.4.1 |
| Image Loading | stb_image |
| Font Rendering | stb_truetype |

---

## Architecture Overview

```
EnJinn/
├── core/
│   ├── enjinnEditor/      # Editor UI (Hierarchy, Inspector, Scene View)
│   ├── enjinnRuntime/     # Core runtime systems
│   ├── enjinnSTD/         # Standard utilities (allocators, file I/O, logging)
│   └── sharedRuntime/     # Scene Graph, Components, Asset Manager
├── gameplay/
│   └── containers/        # Game modules (hot-reloadable DLLs)
├── resources/             # Game assets (textures, shaders, maps)
└── thirdparty/            # External libraries (GLFW, ImGui, Box2D, etc.)
```

### Container Architecture

The engine uses a unique **Container** pattern where each game is a separate DLL that can be loaded/unloaded at runtime:

```cpp
struct Container {
    virtual bool create(RequestedContainerInfo& info) = 0;
    virtual bool update(Input input, WindowState state) = 0;
    virtual void destruct() = 0;
    virtual Scene* getScene() { return nullptr; }
};
```

This enables:
- ✅ Hot-reload during development (no engine restart needed)
- ✅ Multiple games running simultaneously in separate windows
- ✅ Memory isolation between game modules

### Scene Graph System

```cpp
struct Node {
    std::string name;
    Node* parent;
    std::vector<Node*> children;
    std::vector<Component*> components;
    glm::vec3 localPosition;
    glm::quat localRotation;
    glm::vec3 localScale;
};

struct Component {
    virtual void onCreate() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRender() {}
    virtual void onInspector() {}  // Editor UI
};
```

---

## Development Roadmap

### Phase 1: Foundation (Weeks 1-3)
- [ ] Window creation and OpenGL context
- [ ] Basic 2D rendering pipeline
- [ ] Input handling system
- [ ] ImGui integration for debug UI

### Phase 2: Core Systems (Weeks 4-6)
- [ ] Container/DLL hot-reload architecture
- [ ] Custom memory allocators
- [ ] Asset loading and caching
- [ ] Scene Graph implementation

### Phase 3: Editor (Weeks 7-9)
- [ ] Hierarchy panel with node tree
- [ ] Inspector panel for component editing
- [ ] Scene view with camera controls
- [ ] Console and logging system

### Phase 4: Physics & Gameplay (Weeks 10-12)
- [ ] Box2D physics integration
- [ ] Collision detection and response
- [ ] Character controller implementation
- [ ] Demo game: Platformer prototype

### Phase 5: Polish & Demo (Weeks 13-15)
- [ ] Post-processing effects
- [ ] Particle systems
- [ ] Audio integration
- [ ] Final demo game showcase

---

## Demo Games (Planned)

| Game | Description |
|------|-------------|
| **Platformer** | Mario-style side-scroller with tile-based levels |
| **Metroidvania** | Hollow Knight-inspired action platformer |
| **Physics Sandbox** | Angry Birds-style physics puzzle game |
| **3D Showcase** | Minecraft Dungeons-style isometric 3D |

---

## Expected Outcomes

1. **Working Game Engine** – Fully functional editor with rendering, physics, and scene management
2. **Playable Demo** – At least one complete game demonstrating engine capabilities
3. **Documentation** – Comprehensive technical documentation and usage guides
4. **Learning** – Deep understanding of:
   - Graphics programming and rendering pipelines
   - Game architecture and design patterns
   - Memory management and optimization
   - Tool/Editor development

---

## References

- *Game Engine Architecture* by Jason Gregory
- *Real-Time Rendering* by Tomas Akenine-Möller
- Learn OpenGL (https://learnopengl.com)
- Box2D Documentation (https://box2d.org/documentation/)
- Dear ImGui (https://github.com/ocornut/imgui)

---

## Author

**[Your Name]**  
Semester Project – [University Name]  
[Year]

---

> *"The best way to understand how games work is to build the tools that make them."*
