<div align="center">

# EnJinn - The Game Engine


### Modular 2D/3D Game Engine built for engine-level learning and production-style subsystem design.

[![Version](https://img.shields.io/badge/version-1.0.0-green?style=for-the-badge)](https://github.com/jayyvarmaa/EnJinn/releases)
[![License](https://img.shields.io/badge/license-MIT-orange?style=for-the-badge)](LICENSE)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge)](https://github.com/jayyvarmaa/EnJinn)
[![Stars](https://img.shields.io/github/stars/jayyvarmaa/EnJinn?style=for-the-badge&logo=github)](https://github.com/jayyvarmaa/EnJinn/stargazers)

[🚀 Features](#-features) • [📦 Build & Install](#-build--install) • [🎯 Usage](#-usage) • [⚙️ Tech Stack](#️-technology-stack) • [🗺️ Roadmap](#️-roadmap) • [🤝 Support](#-support)

---

</div>

## 📖 About

**EnJinn** is a C++17 game engine and editor focused on **modular runtime architecture**, explicit memory control, and practical tooling for 2D/3D gameplay development. It is structured for engine-level learning and production-style subsystem design.

🔓 **Engine internals made visible** • 🧠 **Explicit memory ownership** • 🔄 **DLL hot reload** • 🎨 **Integrated ImGui editor**

---

## ✨ Features

### 🏗️ **Runtime & Architecture**
- Container-based plugin model for gameplay modules
- Per-container lifecycle management: create, update, destruct
- Deterministic frame pipeline integrating editor and gameplay passes
- Runtime DLL reload support with preserved workflow context

### 🌳 **Scene & Gameplay Systems**
- Node-component scene graph with parent-child hierarchy
- Transform propagation and per-frame component update traversal
- Scene serialization and deserialization workflow
- Shared runtime contracts between engine and gameplay modules

### 🧠 **Memory & Safety**
- Free-list allocator with 8-byte alignment
- Guard-value verification for corruption and double-free detection
- Container-level allocator binding to isolate memory realms
- Fragmentation and allocation metrics for diagnostics

### 🖼️ **Rendering & Tooling**
- OpenGL rendering path with shader compilation diagnostics
- Uniform location caching for reduced runtime lookup overhead
- Editor windows: hierarchy, inspector, scene, profiler, and console
- Logging and assertion mechanisms for development and release builds

---

## 📦 Build & Install

### Option 1: Windows (Visual Studio 2022)

1. Clone the repository:
   ```bash
   git clone https://github.com/jayyvarmaa/EnJinn.git
   cd EnJinn
   ```
2. Bootstrap `vcpkg` if not already installed.
3. Configure and build:
   ```bash
   cd EnJinn
   cmake -B build -G "Visual Studio 17 2022" \
     -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
     -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release --parallel 8
   ```
4. Run the engine:
   ```bash
   .\build\Release\EnJinnCore.exe
   ```

### Option 2: Linux / macOS (Clang or GCC)

```bash
cd EnJinn
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 8
./build/EnJinnCore
```

---

## 🎯 Usage

### Quick Start
1. Launch `EnJinnCore` — the editor opens alongside the runtime loop
2. Use the **Hierarchy** window to create and parent scene nodes
3. Attach components via the **Inspector** window
4. Load a gameplay module (DLL) through the **Container Manager**
5. Iterate with hot-reload — modify gameplay logic and reload without restarting

### Pro Tips
- **Hot reload**: Compile your gameplay DLL and reload mid-session to test changes instantly
- **Memory diagnostics**: Use the Profiler window to track allocator fragmentation and frame metrics
- **Guard values**: Intentionally corrupt a pointer in debug builds to observe guard-value detection in action
- **Scene serialization**: Save and restore scene state between sessions via the Scene panel

---

## ⚙️ Technology Stack

| Category | Technology |
|---|---|
| Language | C++17 |
| Build System | CMake |
| Package Manager | vcpkg |
| Graphics API | OpenGL (via GLAD) |
| Windowing / Input | GLFW |
| UI / Editor | Dear ImGui (docking) |
| Math | GLM |
| Physics | Box2D |
| Assets | stb_image, stb_truetype |

---

## 🗂️ Project Layout

| Path | Responsibility |
|---|---|
| `EnJinn/core/enjinnRuntime` | Main loop, runtime orchestration, container manager, rendering |
| `EnJinn/core/enjinnEditor` | Editor UI and diagnostics windows |
| `EnJinn/core/sharedRuntime` | Shared scene / container / component contracts |
| `EnJinn/core/enjinnSTD` | Allocators, logging, utilities, core support types |
| `EnJinn/gameplay` | Gameplay modules compiled as DLLs |
| `EnJinn/resources` | Textures, models, shaders, scenes, and assets |
| `EnJinn/thirdparty` | Bundled third-party libraries |
| `docs/diagrams` | Architecture and analysis diagrams |

---

## 🛠️ Architectural Highlights

### 📦 Container Pattern
Gameplay is implemented in dynamically loaded modules. This enables hot-reload and isolates gameplay logic from core runtime and editor internals — modules can be swapped at runtime without disrupting the engine state.

### 🧩 Allocator Binding Model
During container update, the runtime binds the container's allocator as the active context, executes gameplay logic, then resets the allocator. This prevents cross-realm allocation leakage and enforces strict memory ownership boundaries.

### 🌿 Scene Graph Design
Scene update performs transform propagation first, followed by component update traversal. This keeps hierarchy correctness and behavior evaluation explicit and deterministic each frame.

### 🖥️ Editor Integration
The editor is frame-synchronous with the runtime and exposes hierarchy, inspection, profiling, and console diagnostics — all within a single unified loop. No separate editor process required.

---

## ✅ Quality & Testing

- Unit and system test plans defined for allocator, scene graph, rendering, and runtime behavior
- Memory safety reinforced with assertions, pointer checks, and guard-value verification
- Profiling outputs include frame-time and container-level metrics
- Build reports target clean compilation and reproducible release outputs

---

## 🗺️ Roadmap

- [ ] Re-establish and automate test suites in CI
- [ ] Improve allocator performance and fragmentation behavior
- [ ] Expand gameplay systems (audio, particles, scripting, AI)
- [ ] Add networking and advanced asset workflows
- [ ] Explore mobile and XR deployment paths

---

## 🤝 Support

### 💖 Show Your Support

If EnJinn has helped you learn engine internals or accelerate your game development, consider:

<div align="center">

[![Star this repo](https://img.shields.io/badge/⭐-Star%20this%20repo-yellow?style=for-the-badge&logo=github)](https://github.com/jayyvarmaa/EnJinn)

**A star helps others discover the project and motivates continued development!** ✨

</div>

### 📬 Connect With The Author

- 💼 **GitHub**: [@jayyvarmaa](https://github.com/jayyvarmaa)
- 🔗 **LinkedIn**: [Jay Varma](https://linkedin.com/in/jayyvarmaa)

### 🐛 Found a Bug?

Please [open an issue](https://github.com/jayyvarmaa/EnJinn/issues) and include:
- Detailed description of the problem
- Steps to reproduce
- Expected vs. actual behavior
- OS, compiler version, and GPU/driver info

### 💡 Feature Requests

Have an idea to improve the engine? [Create a feature request](https://github.com/jayyvarmaa/EnJinn/issues/new) — architecture discussions are welcome!

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## ⚠️ Disclaimer

EnJinn is an independent academic and learning project. It is not affiliated with or endorsed by any commercial game engine vendor. The project is provided as-is with no warranty for production use. Always validate engine behavior in your own target environment.

---

<div align="center">

### 🌟 If you find this project useful, don't forget to give it a star! 🌟

Made [Jay Varma](https://github.com/jayyvarmaa) | 2026

[![GitHub followers](https://img.shields.io/github/followers/jayyvarmaa?style=social)](https://github.com/jayyvarmaa)
[![GitHub stars](https://img.shields.io/github/stars/jayyvarmaa/EnJinn?style=social)](https://github.com/jayyvarmaa/EnJinn/stargazers)

</div>
