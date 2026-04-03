#pragma once
#include <string>
#include <vector>
#include <sceneGraph/component.h>
#include <sceneGraph/components/spriteComponent.h>
#include <sceneGraph/components/physicsComponent.h>
#include "../enjinnRuntime/audio/audioComponents.h"
#include "../enjinnRuntime/particles/particleSystem.h"
#include "../enjinnRuntime/animation/animationSystem.h"
#include "../enjinnRuntime/prefab/prefab.h"
#include "../enjinnRuntime/ai/behaviorTree.h"
#include "../enjinnRuntime/physics/physicsJoints.h"
#include "../enjinnRuntime/rendering/lodSystem.h"
#include "../enjinnRuntime/rendering/cameraComponent.h"
#include "../enjinnRuntime/rendering/lightComponent.h"
#include "../enjinnRuntime/rendering/meshRendererComponent.h"
#include "../enjinnRuntime/rendering/materialComponent.h"
#include "../enjinnRuntime/scripting/scriptComponent.h"

namespace enjinn
{
    inline Component* createComponent(const std::string& name)
    {
        if(name == "SpriteComponent") return new GenericSpriteComponent();
        if(name == "PhysicsComponent") return new GenericPhysicsComponent();
        if(name == "AudioSource") return new AudioSource();
        if(name == "AudioListener") return new AudioListener();
        if(name == "ParticleSystem") return new ParticleSystem();
        if(name == "Animator") return new Animator();
        if(name == "PrefabInstance") return new PrefabInstance();
        if(name == "AIAgent") return new AIAgent();
        if(name == "JointComponent") return new JointComponent();
        if(name == "LODComponent") return new LODComponent();
        if(name == "CameraComponent") return new CameraComponent();
        if(name == "LightComponent") return new LightComponent();
        if(name == "MeshRenderer") return new MeshRendererComponent();
        if(name == "MaterialComponent") return new MaterialComponent();
        if(name == "ScriptComponent") return new ScriptComponent();
        return nullptr;
    }
    
    inline std::vector<std::string> getRegisteredComponentNames()
    {
        return {
            "SpriteComponent",
            "PhysicsComponent",
            "AudioSource",
            "AudioListener",
            "ParticleSystem",
            "Animator",
            "PrefabInstance",
            "AIAgent",
            "JointComponent",
            "LODComponent",
            "CameraComponent",
            "LightComponent",
            "MeshRenderer",
            "MaterialComponent",
            "ScriptComponent"
        };
    }
}
