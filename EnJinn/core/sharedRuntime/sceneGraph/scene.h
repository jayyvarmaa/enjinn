#pragma once
#include "node.h"

namespace enjinn
{
    struct Scene
    {
        Node* rootNode = nullptr;

        Scene();
        ~Scene();

        void update(float deltaTime);
        void render();
    };
}
