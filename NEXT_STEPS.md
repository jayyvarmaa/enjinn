# Next Steps for EnJinn Optimization

## ✅ Completed So Far

1. **Created `.gitignore`**
   - Excludes build artifacts (build/, *.exe, *.dll, etc.)
   - Excludes IDE files (.vs/, .vscode/)
   - Excludes Unity project in docs/cube/
   - **Impact**: Prevents 805 MB+ of build artifacts from being committed

2. **Created `vcpkg.json`**
   - Declares dependencies: GLFW, GLM, ImGui (docking), Box2D, STB
   - Enables automatic dependency installation
   - **Impact**: Repository will only contain source, not vendored libraries

3. **Created `SETUP.md`**
   - Complete setup guide for new developers
   - vcpkg installation instructions
   - Build instructions
   - Troubleshooting section

4. **Created `build.bat` and `build.sh`**
   - Convenient build scripts for Windows and Linux/macOS
   - Automatically checks for vcpkg
   - Configures and builds in one command

5. **Created `MIGRATION.md`**
   - Guide for existing developers to migrate
   - Explains what changed and why
   - Rollback plan if needed

6. **Backed up `CMakeLists.txt`**
   - Original saved as `CMakeLists.txt.backup`
   - Ready for vcpkg migration

---

## 🚧 Next Steps (Requires User Decision)

### Step 1: Handle Unity Project

The `docs/cube/cubeio` Unity project is taking up **292 MB**.

**Options:**
- **A) Delete it** - Remove entirely if it's not needed
- **B) Move it** - Create a separate repository for it
- **C) Ignore it** - Just exclude from .gitignore (already done)

**Recommendation**: Option A or B to reduce repository size.

**Your choice**: _____________

### Step 2: Update CMakeLists.txt (CRITICAL)

The current `CMakeLists.txt` still references vendored libraries. We need to update it to use vcpkg packages.

**Changes needed:**
```diff
- add_subdirectory(thirdparty/glfw-3.3.2)
- add_subdirectory(thirdparty/glm)
- add_subdirectory(thirdparty/imgui-docking)
- add_subdirectory(thirdparty/box2d-2.4.1)
- add_subdirectory(thirdparty/stb_image)
- add_subdirectory(thirdparty/stb_truetype)

+ find_package(glfw3 CONFIG REQUIRED)
+ find_package(glm CONFIG REQUIRED)
+ find_package(imgui CONFIG REQUIRED)
+ find_package(box2d CONFIG REQUIRED)
+ find_package(Stb REQUIRED)
```

**Status**: Ready to implement, but waiting to ensure you have vcpkg installed first.

**Your approval needed**: Should I update CMakeLists.txt now? Y/N: _____________

### Step 3: Install vcpkg (USER ACTION REQUIRED)

Before we can test the new build system, you need to install vcpkg:

```powershell
# 1. Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg

# 2. Bootstrap
.\bootstrap-vcpkg.bat

# 3. Set environment variable
setx VCPKG_ROOT "C:\vcpkg"

# 4. RESTART YOUR TERMINAL
```

**Status**: ⏸️ Waiting for you to install vcpkg

**Confirmation needed**: Have you installed vcpkg? Y/N: _____________

### Step 4: Remove Vendored Dependencies

Once vcpkg is working and the build succeeds, we can remove these directories:

```
thirdparty/glfw-3.3.2/      (→ vcpkg)
thirdparty/glm/             (→ vcpkg) 
thirdparty/imgui-docking/   (→ vcpkg)
thirdparty/box2d-2.4.1/     (→ vcpkg)
thirdparty/stb_image/       (→ vcpkg)
thirdparty/stb_truetype/    (→ vcpkg)
```

**Savings**: ~15 MB from repository

**Status**: ⏸️ Waiting for successful vcpkg build

### Step 5: Clean Build Directory

Remove the existing build directory from version control:

```powershell
cd d:\Projects\EnJinn\EnJinn
Remove-Item -Recurse -Force .\build\
```

**Savings**: ~805 MB from repository

**Status**: ⏸️ Can do this now if you confirm

**Confirmation needed**: Should I clean the build directory? Y/N: _____________

### Step 6: Handle docs/cube Directory

Remove or exclude the Unity project:

```powershell
cd d:\Projects\EnJinn\EnJinn
Remove-Item -Recurse -Force .\docs\cube\
```

**Savings**: ~292 MB from repository

**Status**: ⏸️ Waiting for your decision on Unity project

**Confirmation needed**: Should I delete docs/cube/? Y/N: _____________

---

## 🎯 Expected Final Results

### Repository Size Breakdown

| Component | Before | After | Savings |
|-----------|--------|-------|---------|
| **Source Code** | ~2 MB | ~2 MB | - |
| **Third-Party** | 18 MB | 3 MB | 15 MB |
| **Build Artifacts** | 805 MB | 0 MB | 805 MB |
| **Resources** | 426 MB | 426 MB | - |
| **Docs/Unity** | 292 MB | 0 MB | 292 MB |
| **Total** | **1543 MB** | **~431 MB** | **~1112 MB (72%)** |

If we also handle resources (Git LFS or separate repo):

| Component | After (with LFS) |
|-----------|------------------|
| Source + Custom Libs | ~5 MB |
| Resources (LFS pointers) | ~1 MB |
| **Total Clone Size** | **~50 MB** ✅ |

### Build Process

**Old way:**
```powershell
cd EnJinn
mkdir build
cd build
cmake ..
cmake --build .
```

**New way:**
```powershell
cd EnJinn
.\build.bat
```

Much simpler! 🎉

---

## 📋 Quick Decision Checklist

Please answer these questions so I can proceed:

- [ ] **vcpkg installed?** Y/N
- [ ] **Update CMakeLists.txt?** Y/N
- [ ] **Delete build/ directory?** Y/N
- [ ] **Delete docs/cube/?** Y/N (or specify alternative)
- [ ] **Handle resources with Git LFS?** Y/N (optional, for even smaller repo)

---

## 🔧 Testing Plan (After Above Steps)

1. **Clean build test**
   ```powershell
   cd d:\Projects\EnJinn
   .\build.bat
   ```

2. **Run the engine**
   ```powershell
   cd EnJinn\build\Debug
   .\EnJinnCore.exe
   ```

3. **Verify all features work**
   - Editor launches ✓
   - Scene rendering ✓
   - ImGui UI ✓
   - Physics simulation ✓
   - Hot reload ✓

4. **Measure repository size**
   ```powershell
   cd d:\Projects\EnJinn
   git gc
   # Check .git directory size
   ```

5. **Fresh clone test**
   - Clone to new directory
   - Follow SETUP.md
   - Verify it builds successfully

---

## 📦 Optional: Git LFS for Resources

If you want to make the repository even smaller (Godot-style ~50 MB clone):

1. **Install Git LFS**
   ```powershell
   git lfs install
   ```

2. **Create `.gitattributes`** (I can do this)
   ```gitattributes
   *.png filter=lfs diff=lfs merge=lfs -text
   *.jpg filter=lfs diff=lfs merge=lfs -text
   *.wav filter=lfs diff=lfs merge=lfs -text
   *.mp3 filter=lfs diff=lfs merge=lfs -text
   ```

3. **Migrate existing files**
   ```powershell
   git lfs migrate import --include="*.png,*.jpg,*.wav,*.mp3"
   ```

**Result**: Resources stored as small pointers, downloaded on-demand

**Status**: ⏸️ Optional feature, let me know if you want this

---

## 🎬 What Happens Next?

Once you answer the questions above, I will:

1. ✅ Update `CMakeLists.txt` for vcpkg (if approved)
2. ✅ Clean build directory (if approved)
3. ✅ Remove docs/cube/ (if approved)
4. ✅ Test the new build system
5. ✅ Create a verification walkthrough
6. ✅ Update README.md with new workflow

**Estimated time**: 30-60 minutes (including first vcpkg build)

---

*Ready to proceed? Let me know your decisions on the checklist above!* 🚀
