# EnJinn: Modular 2D/3D Game Engine

EnJinn is a C++17 game engine and editor focused on modular runtime architecture, explicit memory control, and practical tooling for 2D/3D gameplay development.

The project is structured for engine-level learning and production-style subsystem design, including:

- Runtime container orchestration with hot-reloadable gameplay modules
- Scene graph and component model for hierarchical game logic
- Custom memory allocators with guard-value safety checks
- Integrated Dear ImGui editor for diagnostics and inspection
- OpenGL rendering path for both 2D and 3D workloads

## Why EnJinn

EnJinn was built to make engine internals visible and controllable, instead of abstracting away key systems behind monolithic tooling.

Core motivations:

- Understand rendering, update loops, and engine state transitions directly
- Enforce memory ownership and isolation across gameplay modules
- Support iterative gameplay development through DLL hot reload
- Build practical editor and diagnostics workflows around runtime systems

## Core Capabilities

### Runtime and Architecture

- Container-based plugin model for gameplay modules
- Per-container lifecycle management: create, update, destruct
- Deterministic frame pipeline integrating editor and gameplay passes
- Runtime DLL reload support with preserved workflow context

### Scene and Gameplay Systems

- Node-component scene graph with parent-child hierarchy
- Transform propagation and per-frame component update traversal
- Scene serialization and deserialization workflow
- Shared runtime contracts between engine and gameplay modules

### Memory and Safety

- Free-list allocator with 8-byte alignment
- Guard-value verification for corruption and double-free detection
- Container-level allocator binding to isolate memory realms
- Fragmentation and allocation metrics for diagnostics

### Rendering and Tooling

- OpenGL rendering with shader compilation diagnostics
- Uniform location caching for reduced runtime lookup overhead
- Editor windows for hierarchy, inspector, scene, profiler, and console
- Logging and assertion mechanisms for development and release diagnostics

## Technology Stack

| Category | Technology |
|---|---|
| Language | C++17 |
| Build System | CMake |
| Package Manager | vcpkg |
| Graphics API | OpenGL (via GLAD) |
| Windowing/Input | GLFW |
| UI/Editor | Dear ImGui (docking) |
| Math | GLM |
| Physics | Box2D |
| Assets | stb_image, stb_truetype |

## High-Level Project Layout

| Path | Responsibility |
|---|---|
| EnJinn/core/enjinnRuntime | Main loop, runtime orchestration, container manager, rendering |
| EnJinn/core/enjinnEditor | Editor UI and diagnostics windows |
| EnJinn/core/sharedRuntime | Shared scene/container/component contracts |
| EnJinn/core/enjinnSTD | Allocators, logging, utilities, core support types |
| EnJinn/gameplay | Gameplay modules compiled as DLLs |
| EnJinn/resources | Textures, models, shaders, scenes and assets |
| EnJinn/thirdparty | Bundled third-party libraries |
| docs/diagrams | Architecture and analysis diagrams used in project docs |

## Build and Run

### Windows (Visual Studio 2022)

1. Change into the engine directory.
2. Configure with CMake using the vcpkg toolchain.
3. Build the Release configuration.
4. Run the generated EnJinnCore executable.

Commands:

    cd EnJinn
    cmake -B build -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release --parallel 8
    .\build\Release\EnJinnCore.exe

### Linux/macOS (Clang or GCC)

Commands:

    cd EnJinn
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build build --parallel 8
    ./build/EnJinnCore

## Architectural Highlights

### Container Pattern

Gameplay is implemented in dynamically loaded modules. This enables hot reload and isolates gameplay logic from core runtime/editor internals.

### Allocator Binding Model

During container update, the runtime binds the container allocator as active, executes gameplay update, then resets allocator context. This prevents cross-realm allocation leakage.

### Scene Graph Design

Scene update performs transform propagation followed by component update traversal, keeping hierarchy correctness and behavior evaluation explicit.

### Editor Integration

The editor is frame-synchronous with runtime and exposes hierarchy, inspection, profiling, and console diagnostics in one loop.

## Quality, Testing, and Security Summary

- Unit and system test plans were defined for allocator, scene graph, rendering, and runtime behavior
- Memory safety is reinforced with assertions, pointer checks, and guard-value verification
- Profiling outputs include frame-time and container-level metrics
- Build reports target clean compilation and reproducible release outputs

## Future Roadmap

- Re-establish and automate test suites in CI
- Improve allocator performance and fragmentation behavior
- Expand gameplay systems (audio, particles, scripting, AI)
- Add networking and advanced asset workflows
- Explore mobile and XR deployment paths

## Repository Notes

Generated report artifacts and academic export files are intentionally excluded through gitignore rules to keep the repository focused on source, assets, and maintainable documentation.

## Author

Jay Varma
2026
