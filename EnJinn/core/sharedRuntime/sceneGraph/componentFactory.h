#pragma once
#include <string>
#include <sceneGraph/component.h>
#include <sceneGraph/components/spriteComponent.h>
#include <sceneGraph/components/physicsComponent.h>

namespace enjinn
{
    inline Component* createComponent(const std::string& name)
    {
        if(name == "SpriteComponent") return new GenericSpriteComponent();
        if(name == "PhysicsComponent") return new GenericPhysicsComponent();
        return nullptr;
    }
}
