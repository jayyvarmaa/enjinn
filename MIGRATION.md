# vcpkg Migration Guide

This document explains how to migrate existing EnJinn setups to the new vcpkg-based dependency management.

## What Changed?

### Before (Old System)
- All third-party libraries vendored in `thirdparty/`
- Repository size: ~1.5-2 GB
- Build directory included in repo (bad practice)
- Manual dependency updates required

### After (New System with vcpkg)
- Common libraries managed by vcpkg (GLFW, GLM, ImGui, Box2D, STB)
- Custom libraries kept in `thirdparty/`
- Repository size: ~50 MB
- Build directory excluded via `.gitignore`
- Automatic dependency management via `vcpkg.json`

## Migration Steps

### For Fresh Clones (New Developers)

Simply follow the [SETUP.md](SETUP.md) instructions. No migration needed!

### For Existing Developers

If you have an existing EnJinn workspace, follow these steps:

#### 1. Backup Your Work
```powershell
# Commit any uncommitted changes
git add .
git commit -m "Backup before vcpkg migration"
```

#### 2. Install vcpkg
```powershell
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat

# Set environment variable (restart terminal after this)
setx VCPKG_ROOT "C:\vcpkg"
```

**IMPORTANT**: Restart your terminal/IDE after setting VCPKG_ROOT!

#### 3. Clean Old Build Artifacts
```powershell
cd d:\Projects\EnJinn\EnJinn

# Remove old build directory
Remove-Item -Recurse -Force .\build\

# Optional: Remove old third-party binaries (they'll be excluded by .gitignore now)
# Only do this after confirming vcpkg works!
```

#### 4. Pull Latest Changes
```powershell
cd d:\Projects\EnJinn

# Pull the new vcpkg configuration
git pull origin main
```

You should now have:
- `.gitignore` (excludes build artifacts)
- `vcpkg.json` (dependency manifest)
- `SETUP.md` (setup instructions)
- `build.bat` (build script)
- Updated `CMakeLists.txt` (vcpkg integration) - COMING SOON

#### 5. Build with vcpkg

**Option A: Using the build script (easiest)**
```powershell
cd d:\Projects\EnJinn
.\build.bat
```

**Option B: Manual CMake**
```powershell
cd d:\Projects\EnJinn\EnJinn
mkdir build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Debug
```

#### 6. First Build Notes

The **first build** will take longer because vcpkg needs to:
1. Download dependency source code
2. Build dependencies from source
3. Cache binaries for future use

**Expected first build time**: 10-20 minutes (depending on internet speed and CPU)

**Subsequent builds**: Much faster (~2-5 minutes) because dependencies are cached

## Troubleshooting

### "VCPKG_ROOT not found"
- Ensure you set the environment variable: `setx VCPKG_ROOT "C:\vcpkg"`
- **Restart your terminal/IDE** after setting it
- Verify: `echo %VCPKG_ROOT%` should output `C:\vcpkg`

### "CMake can't find packages"
- Ensure you're using the vcpkg toolchain file:
  ```
  -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
  ```
- Delete `build/` and reconfigure from scratch

### Build errors with ImGui
- The ImGui vcpkg package may have slightly different include paths
- If you see include errors, check if you need to update:
  - `#include <imgui.h>` (correct for vcpkg)
  - `#include "imgui/imgui.h"` (old path)

### "Stb not found"
- vcpkg provides STB as a single package
- The headers will be at:
  - `stb_image.h` and `stb_truetype.h`
- No code changes should be needed

## What Gets Removed?

Once the vcpkg migration is complete and tested, these directories will be removed from the repository:

- ❌ `thirdparty/glfw-3.3.2/` → Managed by vcpkg
- ❌ `thirdparty/glm/` → Managed by vcpkg
- ❌ `thirdparty/imgui-docking/` → Managed by vcpkg
- ❌ `thirdparty/box2d-2.4.1/` → Managed by vcpkg
- ❌ `thirdparty/stb_image/` → Managed by vcpkg (as part of `stb` package)
- ❌ `thirdparty/stb_truetype/` → Managed by vcpkg (as part of `stb` package)

These will remain (custom libraries):

- ✅ `thirdparty/glad/` (project-specific OpenGL loader)
- ✅ `thirdparty/gl2d/` (custom 2D rendering)
- ✅ `thirdparty/gl3d/` (custom 3D rendering)
- ✅ `thirdparty/glui/` (custom UI library)
- ✅ `thirdparty/ph2d/` (custom physics wrapper)
- ✅ `thirdparty/profilerLib/` (profiler)
- ✅ `thirdparty/safeSafe/` (serialization)
- ✅ `thirdparty/sushi/` (utilities)

## Git Best Practices

After migration:

### Do NOT commit
- ❌ `build/` directory
- ❌ `vcpkg_installed/` directory  
- ❌ Any `*.exe`, `*.dll`, `*.lib` files
- ❌ CMake cache files

### DO commit
- ✅ Source code (`.cpp`, `.h`)
- ✅ CMakeLists.txt
- ✅ `vcpkg.json` (dependency manifest)
- ✅ `.gitignore`
- ✅ Documentation (`.md` files)

The `.gitignore` file handles this automatically!

## Benefits After Migration

1. **Smaller Repository**
   - Clone size: ~50 MB (down from 1.5-2 GB!)
   - Faster git operations

2. **Easier Updates**
   - Update dependencies: `vcpkg upgrade --no-dry-run`
   - Version locked in `vcpkg.json`

3. **Better Collaboration**
   - New developers don't download build artifacts
   - Consistent dependency versions across team

4. **Cross-Platform**
   - vcpkg works on Windows, Linux, macOS
   - Same `vcpkg.json` works everywhere

5. **Industry Standard**
   - Used by Microsoft, many open-source projects
   - Well-documented and maintained

## Rollback Plan

If you encounter issues and need to rollback:

```powershell
# Restore old CMakeLists.txt
cd d:\Projects\EnJinn\EnJinn
copy CMakeLists.txt.backup CMakeLists.txt

# Remove vcpkg configuration
cd d:\Projects\EnJinn
git checkout HEAD -- vcpkg.json .gitignore

# Clean and rebuild with old system
Remove-Item -Recurse -Force .\EnJinn\build\
# Then build as you did before
```

## Questions?

If you encounter issues during migration:

1. Check this guide first
2. Review [SETUP.md](SETUP.md) for setup instructions
3. Try a clean build (delete `build/` and reconfigure)
4. Ask on the project's issue tracker

---

*Last updated: 2026-01-20*
