#pragma once
#include <logs/log.h>
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <enjinnImgui/enjinnImgui.h>


#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR


namespace enjinn
{


	struct LogWindow
	{

		void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);

		void update(enjinn::LogManager &logManager, bool &open);

		static constexpr char *ICON = ICON_FK_COMMENT_O;
		static constexpr char *NAME = "logs";
		static constexpr char *ICON_NAME = ICON_FK_COMMENT_O " logs";
		bool autoScroll = true;
		ImGuiTextFilter filter;

		int imguiId = 0;
	};

}


#endif