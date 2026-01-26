#include <sceneGraph/component.h>
#include <gl2d/gl2d.h>
#include <imgui.h>
#include <vector>
#include <string>
#include <assetManager.h>

struct GenericSpriteComponent : public enjinn::Component
{
    std::string texturePath = "";
    glm::vec4 color = {1,1,1,1};
    bool flip = false;
    int layer = 0;

    void onInspector() override
    {
        char buffer[256];
        strncpy(buffer, texturePath.c_str(), sizeof(buffer));
        if(ImGui::InputText("Texture", buffer, sizeof(buffer)))
        {
            texturePath = buffer;
        }

        ImGui::ColorEdit4("Color", &color[0]);
        ImGui::Checkbox("Flip", &flip);
        ImGui::InputInt("Layer", &layer);
    }

    void serialize(sfs::SafeSafeKeyValueData& data) override
    {
        data.setString("texturePath", texturePath);
        data.setVec4("color", color.x, color.y, color.z, color.w);
        data.setBool("flip", flip);
        data.setInt("layer", layer);
    }

    void deserialize(sfs::SafeSafeKeyValueData& data) override
    {
        data.getString("texturePath", texturePath);
        data.getVec4("color", color.x, color.y, color.z, color.w);
        data.getBool("flip", flip);
        data.getInt("layer", layer);
    }
    
    gl2d::Texture getTexture()
    {
        return enjinn::AssetManager::getTexture(texturePath);
    }

    const char* getComponentName() override { return "SpriteComponent"; }
};
