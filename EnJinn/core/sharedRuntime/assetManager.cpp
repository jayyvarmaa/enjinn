#include "assetManager.h"
#include <engineLibraresSupport/engineGL2DSupport.h>
#include <iostream>

namespace enjinn
{
    RequestedContainerInfo* AssetManager::containerInfo = nullptr;
    std::map<std::string, ::gl2d::Texture> AssetManager::textureCache;

    void AssetManager::init(RequestedContainerInfo* info)
    {
        containerInfo = info;
    }

    ::gl2d::Texture AssetManager::getTexture(const std::string& path)
    {
        if (path.empty()) return {};

        auto it = textureCache.find(path);
        if (it != textureCache.end())
        {
            return it->second;
        }

        if (containerInfo)
        {
            // Assuming texture paths are relative to ENJINN_RESOURCES_PATH or handled by engineGL2DSupport
            // But engineGL2DSupport usually takes full path if it just uses readBinary.
            // However, SilkSong was doing: ENJINN_RESOURCES_PATH "hollowknight/" + path
            // For generic use, the path stored in the component should probably be: "hollowknight/bush.png"
            // And we prepend ENJINN_RESOURCES_PATH here.
            
            std::string fullPath = std::string(ENJINN_RESOURCES_PATH) + path;
            
            ::gl2d::Texture t = gl2d::loadTexture(fullPath.c_str(), *containerInfo);
            if (t.id != 0)
            {
                textureCache[path] = t;
                return t;
            }
            else
            {
               // Fallback or error logging? 
               // std::cout << "Failed to load texture: " << fullPath << "\n";
            }
        }
        return {};
    }

    void AssetManager::clear()
    {
        // GL2D textures are just integers, but we might want to glDeleteTextures if we were managing lifetime strictly.
        // For now, this is just a cache clear.
        textureCache.clear();
    }
}
