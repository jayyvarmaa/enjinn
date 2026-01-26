# EnJinn Setup Guide

## Prerequisites

### 1. Install vcpkg

vcpkg is a cross-platform package manager for C/C++ libraries. It will automatically download and build all dependencies.

```powershell
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Add to PATH (optional but recommended)
# This allows you to run vcpkg from anywhere
```

### 2. Set Environment Variable

Set the `VCPKG_ROOT` environment variable:

**Option A: Permanently (Recommended)**
```powershell
# PowerShell (Run as Administrator)
[System.Environment]::SetEnvironmentVariable('VCPKG_ROOT', 'C:\vcpkg', 'User')
```

**Option B: Current Session Only**
```powershell
$env:VCPKG_ROOT = "C:\vcpkg"
```

### 3. Install CMake

Download and install CMake 3.16 or later from [cmake.org/download](https://cmake.org/download/)

Ensure CMake is added to your PATH during installation.

### 4. Install Visual Studio

Install Visual Studio 2019 or later with:
- Desktop development with C++
- Windows 10 SDK
- CMake tools for Windows

---

## Building EnJinn

### Quick Start

1. **Clone the repository**
   ```powershell
   git clone https://github.com/yourusername/EnJinn.git
   cd EnJinn
   ```

2. **Create build directory**
   ```powershell
   mkdir build
   cd build
   ```

3. **Configure with CMake**
   
   The vcpkg manifest (`vcpkg.json`) will automatically install all dependencies on first configure.
   
   ```powershell
   cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
   ```
   
   Or if using Visual Studio:
   ```powershell
   cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"
   ```

4. **Build**
   ```powershell
   cmake --build . --config Debug
   ```
   
   Or open `EnJinn.sln` in Visual Studio and build from there.

5. **Run**
   ```powershell
   .\Debug\EnJinnCore.exe
   ```

### Alternative: Using build.bat

A convenient build script is provided:

```powershell
.\build.bat
```

This will automatically configure and build the project.

---

## Project Size Comparison

After this optimization, your local workspace will be much smaller:

| Stage | Size | Notes |
|-------|------|-------|
| **Fresh Clone** | ~50 MB | Source code only (no dependencies, no build artifacts) |
| **After Configure** | ~150 MB | vcpkg downloads dependencies to `vcpkg_installed/` |
| **After Build** | ~850 MB | Includes Debug build artifacts |
| **Dependencies Cache** | Varies | vcpkg caches binaries globally (shared across projects) |

**Previous Size**: ~1.5-2 GB 😱  
**New Size**: ~50 MB ✅

---

## Dependencies

EnJinn uses the following third-party libraries:

### Managed by vcpkg (auto-installed)
- **GLFW 3.3+** - Windowing and input
- **GLM** - Math library
- **Dear ImGui** (docking branch) - UI framework
- **Box2D** - 2D physics engine
- **stb_image & stb_truetype** - Image and font loading

### Custom libraries (included in repo)
- **GLAD** - OpenGL loader
- **gl2d** - Custom 2D rendering library
- **gl3d** - Custom 3D rendering library
- **glui** - Custom UI library
- **ph2d** - Custom physics wrapper
- **profilerLib** - Performance profiling
- **safeSafe** - Serialization system
- **sushi** - Utility library

---

## Troubleshooting

### vcpkg not found
- Ensure `VCPKG_ROOT` environment variable is set correctly
- Restart your terminal/IDE after setting environment variables
- Check that vcpkg.exe exists at `%VCPKG_ROOT%\vcpkg.exe`

### Dependencies fail to install
- Check your internet connection (vcpkg downloads sources)
- Try clearing vcpkg cache: `vcpkg remove --outdated --recurse`
- Ensure you have enough disk space (~500 MB for dependencies)

### CMake can't find packages
- Verify you're using the vcpkg toolchain file:
  ```
  -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
  ```
- Delete `build/` directory and reconfigure from scratch

### Build errors
- Ensure you have Visual Studio 2019+ with C++ tools
- Try building with `--config Debug` explicitly
- Check that all submodules are initialized

---

## For Existing Developers

If you were using the old setup:

1. **Delete old build directory**
   ```powershell
   rm -r -Force .\EnJinn\build\
   ```

2. **Install vcpkg** (see prerequisites above)

3. **Reconfigure and rebuild** (see Building EnJinn above)

The old third-party libraries in `EnJinn/thirdparty/` are being phased out. Some custom libraries will remain, but GLFW, GLM, ImGui, Box2D, and STB are now managed by vcpkg.

---

## Next Steps

Once built successfully:

1. Read [README.md](README.md) for project overview
2. Check [docs/](docs/) for architecture documentation
3. Explore sample containers in `gameplay/containers/`
4. Start building your game!

---

## Contributing

When contributing to EnJinn:

1. **Never commit build artifacts** - The `.gitignore` excludes these automatically
2. **Never commit `vcpkg_installed/`** - Dependencies are auto-installed from manifest
3. **Update `vcpkg.json`** if adding new dependencies
4. **Keep custom libraries** in `thirdparty/` if they're project-specific

---

## Questions?

- Check the [Issues](https://github.com/yourusername/EnJinn/issues) page
- Read the [Documentation](docs/)
- Contact the maintainers

Happy game development! 🎮
