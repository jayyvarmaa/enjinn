#pragma once
#include <enjinnConfig.h>
#include <safeSave/safeSave.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "component.h"

namespace enjinn
{
    // Pool allocator for components - reduces heap fragmentation and keeps RAM predictable
    struct ComponentPool
    {
        static constexpr size_t POOL_SIZE = 1024 * 1024; // 1 MB pool
        static constexpr size_t MAX_ALLOC = 4096;        // Max single component size
        
        static ComponentPool& getInstance()
        {
            static ComponentPool pool;
            return pool;
        }
        
        void* allocate(size_t size)
        {
            // Align to 16 bytes
            size = (size + 15) & ~15;
            
            if (size > MAX_ALLOC || offset + size > POOL_SIZE)
            {
                // Fall back to heap for oversized or when pool is full
                return std::malloc(size);
            }
            
            void* ptr = pool + offset;
            offset += size;
            return ptr;
        }
        
        void deallocate(void* ptr)
        {
            // Check if this pointer is within our pool
            if (ptr >= pool && ptr < pool + POOL_SIZE)
            {
                // Pool memory is reclaimed on reset, not per-dealloc
                return;
            }
            std::free(ptr);
        }
        
        void reset()
        {
            offset = 0;
        }
        
    private:
        ComponentPool() { std::memset(pool, 0, POOL_SIZE); }
        alignas(16) char pool[POOL_SIZE];
        size_t offset = 0;
    };

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
            void* mem = ComponentPool::getInstance().allocate(sizeof(T));
            T* c = new (mem) T(std::forward<Args>(args)...);
            c->node = this;
            components.push_back(c);
            c->onCreate();
            return c;
        }

        virtual ~Node();
    };
}
