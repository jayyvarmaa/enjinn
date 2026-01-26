# FontAwesome 6 Setup for EnJinn

EnJinn now supports **FontAwesome 6** icons for a sharper, more modern UI!

## Quick Setup

### 1. Download FontAwesome 6 Free

Visit: **https://fontawesome.com/download**

- Click **"Free for Web"**
- Download the ZIP file (fontawesome-free-6.x.x-web.zip)

### 2. Extract the Font File

From the downloaded ZIP:

1. Navigate to `fontawesome-free-6.x.x-web/webfonts/`
2. Find the file: **`fa-solid-900.ttf`** 
3. Copy it to: **`EnJinn/resources/fa-solid-900.ttf`**

### 3. Build and Run

That's it! The engine will automatically detect and use FontAwesome 6.

```powershell
cd EnJinn
mkdir build && cd build
cmake ..
cmake --build . --config Debug
.\Debug\EnJinnCore.exe
```

---

## Fallback Mode

If `fa-solid-900.ttf` is not found, EnJinn will automatically fall back to FontAwesome 4 (`fontawesome-webfont.ttf`), so the editor will still work!

---

## What Changed?

- **Icons**: Now using FA6 glyph codes (sharper rendering)
- **Font Size**: Increased from 12px to 14px (better readability)
- **Backward Compatibility**: Old `ICON_FK_*` aliases still work!

---

## Icon Usage

Use icons in your code:

```cpp
#include <IconsFontAwesome6.h>

// New FA6 style
ImGui::Button(ICON_FA_FOLDER " Open");
ImGui::Text(ICON_FA_GEAR " Settings");

// Old aliases still work
ImGui::Button(ICON_FK_FOLDER_O " Open");  // Maps to ICON_FA_FOLDER
```

---

## Available Icons

See the full icon list:
- **Header**: `EnJinn/core/sharedRuntime/enjinnImgui/IconsFontAwesome6.h`
- **Online**: https://fontawesome.com/icons

Common icons included:
- `ICON_FA_FILE`, `ICON_FA_FOLDER`
- `ICON_FA_GEAR`, `ICON_FA_GEARS`
- `ICON_FA_EYE`, `ICON_FA_EYE_SLASH`
- `ICON_FA_ROTATE_LEFT`, `ICON_FA_ROTATE_RIGHT` (Undo/Redo)
- `ICON_FA_GAMEPAD`, `ICON_FA_TERMINAL`
- `ICON_FA_CIRCLE_INFO`, `ICON_FA_TRIANGLE_EXCLAMATION`
- ...and 100+ more!

---

## Troubleshooting

**"Icons showing as boxes"**
- Make sure `fa-solid-900.ttf` is in `EnJinn/resources/`
- Rebuild the project (fonts load at startup)

**"Where do I get other FA6 fonts?"**
- EnJinn uses **Solid** style by default
- For Regular or Brands, download those .ttf files and modify `enjinnImgui.cpp`

---

Happy icon-ing! ✨
