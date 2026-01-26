#pragma once
#include <enjinnConfig.h>
#include <enjinnImgui/enjinnImgui.h>

namespace enjinn
{
    struct SceneViewWindow
    {
        static constexpr const char* ICON_NAME = ICON_FK_PICTURE_O " Scene";
        
        void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);
        void update(unsigned int textureId, bool &open);

    private:
        int windowId = 0;
    };
}
