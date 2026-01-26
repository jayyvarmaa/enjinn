#pragma once


#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include <IconsFontAwesome6.h>
#include <shortcutApi/shortcutApi.h>
#include <enjinnImgui/enjinnImgui.h>

namespace enjinn
{

	struct EditShortcutsWindow
	{


		void init(enjinn::enjinnImgui::ImGuiIdsManager &imguiIdManager);

		void update(enjinn::ShortcutManager &shortcutManager, bool &open);

		static constexpr char *ICON = ICON_FK_PENCIL_SQUARE;
		static constexpr char *NAME = "Edit Shortcuts...";
		static constexpr char *ICON_NAME = ICON_FK_PENCIL_SQUARE " Edit Shortcuts...";

		int imguiId = 0;
	};



};

#endif