#pragma once
#include "node.h"
#include <string>

namespace enjinn
{
    struct Scene
    {
        Node* rootNode = nullptr;

        Scene();
        ~Scene();

        void update(float deltaTime);
        void render();

        // Scene save/load
        bool saveScene(const char* filepath);
        bool loadScene(const char* filepath);
        
        // Clear and rebuild from scratch
        void clear();
        
        std::string scenePath;
    };
}
