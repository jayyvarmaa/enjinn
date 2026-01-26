# Build Error Fix - Step by Step

## Error Summary
**Main Issue**: `IconsFontAwesome6.h: No such file or directory`

**Why**: Files use `#include <IconsFontAwesome6.h>` but compiler can't find it.

---

## Step-by-Step Fixes

### Step 1: Confirm File Locations
I found these files:
- ✅ `EnJinn/include/IconsFontAwesome6.h` (GOOD - in include path)
- ⚠️ `EnJinn/core/sharedRuntime/enjinnImgui/IconsFontAwesome6.h` (DUPLICATE - delete this)
- ℹ️ `EnJinn/thirdparty/imgui-docking/imgui/IconsForkAwesome.h` (OLD - leave it)

**Action**: Delete the duplicate
```powershell
cd d:\Projects\EnJinn\EnJinn
Remove-Item core\sharedRuntime\enjinnImgui\IconsFontAwesome6.h
```

### Step 2: Check Include Paths in Files
The issue is that some files use **angle brackets** `<IconsFontAwesome6.h>` but it's not in the CMake include directories.

**Option A - Fix CMakeLists.txt** (Proper way):
Add to `EnJinn/CMakeLists.txt` near the top (around line 20-30):
```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
```

**Option B - Use Quotes** (Quick hack):
Change all includes from:
```cpp
#include <IconsFontAwesome6.h>
```
To:
```cpp
#include "IconsFontAwesome6.h"
```

### Step 3: Fix PhaseManager Include
**File**: `EnJinn/core/enjinnEditor/phaseManager/phaseManager.h`

**Find** (line 8):
```cpp
#include <string>
```

**Replace with**:
```cpp
#include <enjinnConfig.h>
#include <string>
```

### Step 4: Rebuild
```powershell
cd d:\Projects\EnJinn\EnJinn\test-build
cmake --build . --config Debug
```

---

## Recommended: Do Steps 1, 2A, and 3

**Step 1**: Remove duplicate file
**Step 2**: Add include directory to CMakeLists.txt
**Step 3**: Fix phaseManager.h include

Then rebuild.

---

## If Still Failing

Run this to see remaining errors:
```powershell
cd test-build
cmake --build . --config Debug 2>&1 | Select-String "error" | Select-Object -First 10
```

Send me the output and I'll help with the next error!

---

**Ready to proceed?** Start with Step 1 and tell me when you're done!
