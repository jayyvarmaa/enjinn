# Building EnJinn

This guide explains how to build the EnJinn game engine. The project supports **two build methods** with identical functionality.

---

## Method 1: Legacy Build (Works Immediately)

This method uses the vendored libraries already included in the repository. **No additional setup required!**

### Steps:

```powershell
# 1. Navigate to project directory
cd d:\Projects\EnJinn\EnJinn

# 2. Create build directory
mkdir build
cd build

# 3. Configure with CMake
cmake ..

# 4. Build
cmake --build . --config Debug

# 5. Run the engine
.\Debug\EnJinnCore.exe
```

### Visual Studio Users:

```powershell
# Generate Visual Studio solution
cmake .. -G "Visual Studio 17 2022"

# Open the solution
start EnJinn.sln
```

Then build and run from Visual Studio as usual.

---

## Method 2: vcpkg Build (Recommended for New Projects)

This method uses vcpkg for modern dependency management. Requires one-time vcpkg installation.

### One-Time Setup:

```powershell
# 1. Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg

# 2. Navigate to vcpkg directory
cd C:\vcpkg

# 3. Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# 4. Set environment variable
setx VCPKG_ROOT "C:\vcpkg"

# 5. IMPORTANT: Restart your terminal/IDE
```

### Build Steps:

**Option A: Using the build script (easiest)**
```powershell
cd d:\Projects\EnJinn
.\build.bat
```

**Option B: Manual**
```powershell
cd d:\Projects\EnJinn\EnJinn
mkdir build
cd build

# Configure with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build . --config Debug

# Run
.\Debug\EnJinnCore.exe
```

### First Build Notes:

- **First build** takes 15-20 minutes (vcpkg downloads and builds dependencies)
- **Subsequent builds** take 2-5 minutes (dependencies are cached)
- Dependencies are cached globally and reused across projects

---

## Comparison

| Aspect | Legacy Build | vcpkg Build |
|--------|-------------|-------------|
| **Setup Time** | 0 min (ready to go) | 10 min (one-time vcpkg install) |
| **First Build** | ~5 min | ~15-20 min |
| **Subsequent Builds** | ~5 min | ~2-5 min |
| **Disk Usage** | Higher (vendored libs in repo) | Lower (deps in vcpkg cache) |
| **Repository Size** | Includes all dependencies | Only source code |
| **Updates** | Manual | `vcpkg upgrade` |
| **Recommended For** | Quick start, existing setups | New projects, teams |

---

## How It Works

The `CMakeLists.txt` **automatically detects** which method you're using:

- If you configure with the vcpkg toolchain file → **vcpkg mode**
- If you configure normally → **legacy mode** (vendored libraries)

Both modes produce **identical binaries** with **identical functionality**!

---

## Troubleshooting

### Legacy Build Issues

**"CMake Error: Could not find GL/gl.h"**
- Install Windows SDK (comes with Visual Studio)
- Make sure "Desktop development with C++" is installed in Visual Studio

**"Cannot open file glfw3.lib"**
- Delete `build/` directory
- Reconfigure and rebuild from scratch

### vcpkg Build Issues

**"vcpkg not found"**
- Ensure `VCPKG_ROOT` environment variable is set
- Restart terminal after setting the variable
- Verify: `echo %VCPKG_ROOT%` should output `C:\vcpkg`

**"Package not found"**
- First configure automatically installs packages from `vcpkg.json`
- If it fails, try: `cd %VCPKG_ROOT% && vcpkg install glfw3  glm imgui[docking-experimental] box2d`

**vcpkg installation hangs**
- Check internet connection
- Try different terminal (sometimes PowerShell vs CMD matters)
- Check firewall/antivirus settings

---

## Build Configurations

### Debug Build (Default)
```powershell
cmake --build . --config Debug
```
- Includes debug symbols
- Assertions enabled
- Slower performance
- Use for development

### Release Build
```powershell
cmake --build . --config Release
```
- Optimized for performance
- No debug symbols
- Use for distribution/testing

---

## Clean Build

If you encounter issues, try a clean build:

```powershell
# Navigate to EnJinn directory
cd d:\Projects\EnJinn\EnJinn

# Remove build directory
Remove-Item -Recurse -Force build

# Rebuild
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

---

## Next Steps

Once built successfully:

1. **Run the engine**: `cd build\Debug && .\EnJinnCore.exe`
2. **Explore the code**: Check out `core/`, `gameplay/`, and `resources/`
3. **Create a game**: Add a new container in `gameplay/containers/`
4. **Read documentation**: See `docs/` for architecture details

---

## Need Help?

- **Setup problems**: See [SETUP.md](SETUP.md)
- **Migration from old setup**: See [MIGRATION.md](MIGRATION.md)
- **General questions**: Check the main [README.md](README.md)
- **Issues**: Open issue on GitHub or contact maintainers

---

Happy game development! 🎮
