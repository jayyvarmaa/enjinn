#pragma once
#include <enjinnConfig.h>
#include <safeSave/safeSave.h>

namespace enjinn
{
    struct Node; // Forward declaration

    struct Component
    {
        Node* node = nullptr;

        virtual void onCreate() {}
        virtual void onUpdate(float deltaTime) {}
        virtual void onRender() {}
        virtual void onInspector() {};
        
        virtual void serialize(sfs::SafeSafeKeyValueData& data) {};
        virtual void deserialize(sfs::SafeSafeKeyValueData& data) {};
        virtual const char* getComponentName() { return "Component"; };

        virtual ~Component() {};
    };
}
