#pragma once
#include <enjinnConfig.h>
#include <safeSave/safeSave.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "component.h"

namespace enjinn
{
    struct Node
    {
        std::string name = "Node";
        bool active = true;
        std::string tag = "Untagged";
        int layer = 0;
        
        Node* parent = nullptr;
        std::vector<Node*> children;
        std::vector<Component*> components;

        glm::vec3 localPosition = {0,0,0};
        glm::quat localRotation = {1,0,0,0};
        glm::vec3 localScale = {1,1,1};

        glm::mat4 globalTransform = glm::mat4(1.f);

        void addChild(Node* child);
        void removeChild(Node* child);
        void updateTransforms();
        
        void serialize(sfs::SafeSafeKeyValueData& data);
        void deserialize(sfs::SafeSafeKeyValueData& data);

        template<typename T, typename... Args>
        T* addComponent(Args&&... args)
        {
            T* c = new T(std::forward<Args>(args)...); // todo: use custom allocator
            c->node = this;
            components.push_back(c);
            c->onCreate();
            return c;
        }

        virtual ~Node();
    };
}
