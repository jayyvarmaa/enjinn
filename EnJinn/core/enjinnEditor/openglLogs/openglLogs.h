#pragma once
#include <IconsFontAwesome6.h>
#include <imgui.h>
#include <enjinnImgui/enjinnImgui.h>
#include <unordered_map>

namespace enjinn
{


	struct OpenglLogsWindow
	{

		void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);

		void update(bool &open);

		static constexpr char *ICON = ICON_FK_EXCLAMATION_TRIANGLE;
		static constexpr char *NAME = "opengl errors";
		static constexpr char *ICON_NAME = ICON_FK_EXCLAMATION_TRIANGLE " opengl errors";

		std::unordered_map<unsigned int, unsigned int> errorsReported;

		int imguiId = 0;
	};

}
