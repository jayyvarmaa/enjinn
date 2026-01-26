#pragma once
#include <string>
#include <map>
#include <gl2d/gl2d.h>
#include <baseContainer.h>

namespace enjinn
{
    class AssetManager
    {
    public:
        static void init(RequestedContainerInfo* info);
        static ::gl2d::Texture getTexture(const std::string& path);
        static void clear();

    private:
        static RequestedContainerInfo* containerInfo;
        static std::map<std::string, ::gl2d::Texture> textureCache;
    };
}
