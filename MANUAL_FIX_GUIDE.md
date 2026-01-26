# MANUAL BUILD FIX - Quick Guide

## Current Issue
Build failing because IconsFontAwesome6.h is in wrong location and PhaseManager needs config include.

## 3 Simple Fixes

### 1. Move Icon Header (or just delete the new file and revert)
Current location: `EnJinn/core/sharedRuntime/enjinnImgui/IconsFontAwesome6.h`
Should be: `EnJinn/include/IconsFontAwesome6.h`

**Option A - Move it**:
```powershell
Move-Item EnJinn/core/sharedRuntime/enjinnImgui/IconsFontAwesome6.h EnJinn/include/IconsFontAwesome6.h
```

**Option B - Just use old icons for now**:
```powershell
# Revert all icon changes
cd EnJinn
git checkout core/
```

### 2. Fix PhaseManager Include  
**File**: `EnJinn/core/enjinnEditor/phaseManager/phaseManager.h`
**Line 8**: Add before `#include <string>`:
```cpp
#include <enjinnConfig.h>
```

### 3. Rebuild
```powershell
cd EnJinn/test-build
cmake --build . --config Debug
```

## If That Fails - Nuclear Option

Delete these new files and just run existing engine:
- `EnJinn/core/enjinnEditor/phaseManager/` (entire folder)
- `EnJinn/core/sharedRuntime/enjinnImgui/IconsFontAwesome6.h`

Then revert icon changes:
```powershell
git checkout EnJinn/core/
```

Then build should work with OLD code.

## What You'll Get

After manual fix:
- Engine runs with current features
- Can test Silksong camera
- Phase system can be added later when build is stable

**Just tell me which option you want and I'll guide you through it!**
