#pragma once
#include <sceneGraph/component.h>
#include <box2d/box2d.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdint>

// Collision layer names (editable per-project)
namespace enjinn
{
    struct CollisionLayers
    {
        static constexpr int MAX_LAYERS = 16;
        static constexpr const char* defaultNames[MAX_LAYERS] = {
            "Default", "Player", "Enemy", "Ground", "Projectile",
            "Trigger", "UI", "Debris", "Layer8", "Layer9",
            "Layer10", "Layer11", "Layer12", "Layer13", "Layer14", "Layer15"
        };
        
        // Layer mask: which layers this layer collides with (bitmask)
        static inline uint16_t collisionMatrix[MAX_LAYERS] = {
            0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
            0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
        };
        
        static uint16_t getCategoryBits(int layer)
        {
            if (layer < 0 || layer >= MAX_LAYERS) return 1;
            return (uint16_t)(1 << layer);
        }
        
        static uint16_t getMaskBits(int layer)
        {
            if (layer < 0 || layer >= MAX_LAYERS) return 0xFFFF;
            return collisionMatrix[layer];
        }
    };
}

struct GenericPhysicsComponent : public enjinn::Component
{
    b2Body* body = nullptr;
    b2World* world = nullptr;
    glm::vec2 size = {1,1};
    
    // Collision filtering
    int collisionLayer = 0;         // 0-15, index into CollisionLayers
    int16_t groupIndex = 0;         // Box2D group index (+ = always collide, - = never)
    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.0f;
    bool isSensor = false;

    void onInspector() override
    {
        if(!body) 
        {
            ImGui::TextColored(ImVec4(1,0,0,1), "Physics Body not initialized!");
            return;
        }

        int item = body->GetType();
        if(ImGui::Combo("Body Type", &item, "Static\0Kinematic\0Dynamic\0"))
        {
            body->SetType((b2BodyType)item);
        }

        if(ImGui::DragFloat2("Size", &size.x, 0.1f))
        {
            updateFixture();
        }
        
        // Collision layer combo
        if (ImGui::Combo("Collision Layer", &collisionLayer,
            enjinn::CollisionLayers::defaultNames[0]))
        {
            // Build combo items string
        }
        // Use a simpler approach for layer selection
        if (ImGui::SliderInt("Layer", &collisionLayer, 0, enjinn::CollisionLayers::MAX_LAYERS - 1))
        {
            updateFixture();
        }
        ImGui::Text("Layer: %s", enjinn::CollisionLayers::defaultNames[collisionLayer]);
        
        if (ImGui::DragFloat("Density", &density, 0.1f, 0.0f, 100.0f))
            updateFixture();
        if (ImGui::DragFloat("Friction", &friction, 0.01f, 0.0f, 1.0f))
            updateFixture();
        if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f, 1.0f))
            updateFixture();
        if (ImGui::Checkbox("Is Sensor", &isSensor))
            updateFixture();
        
        // Debug info
        auto pos = body->GetPosition();
        ImGui::Text("Body Pos: %.2f, %.2f", pos.x, pos.y);
    }

    void updateFixture()
    {
        if(!body) return;
        b2Fixture* f = body->GetFixtureList();
        while(f)
        {
            b2Fixture* next = f->GetNext();
            body->DestroyFixture(f);
            f = next;
        }

        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x / 2, size.y / 2);

        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = density;
        boxFixtureDef.friction = friction;
        boxFixtureDef.restitution = restitution;
        boxFixtureDef.isSensor = isSensor;
        
        // Apply collision layer filtering
        boxFixtureDef.filter.categoryBits = enjinn::CollisionLayers::getCategoryBits(collisionLayer);
        boxFixtureDef.filter.maskBits = enjinn::CollisionLayers::getMaskBits(collisionLayer);
        boxFixtureDef.filter.groupIndex = groupIndex;
        
        body->CreateFixture(&boxFixtureDef);
    }

    // Call this in update loop to sync Node -> Physics or Physics -> Node
    void syncNodeToBody()
    {
        if(body && node)
        {
             // body->SetTransform(b2Vec2(node->localPosition.x, node->localPosition.y), glm::eulerAngles(node->localRotation).z);
        }
    }

    void serialize(sfs::SafeSafeKeyValueData& data) override
    {
        data.setVec2("size", size.x, size.y);
        if(body)
        {
            data.setInt("bodyType", (int)body->GetType());
        }
        data.setInt("collisionLayer", collisionLayer);
        data.setInt("groupIndex", (int)groupIndex);
        data.setFloat("density", density);
        data.setFloat("friction", friction);
        data.setFloat("restitution", restitution);
        data.setInt("isSensor", isSensor ? 1 : 0);
    }

    void deserialize(sfs::SafeSafeKeyValueData& data) override
    {
        data.getVec2("size", size.x, size.y);
        int type = 0;
        if(data.getInt("bodyType", type) == sfs::noError)
        {
        }
        data.getInt("collisionLayer", collisionLayer);
        int gi = 0;
        if (data.getInt("groupIndex", gi) == sfs::noError)
            groupIndex = (int16_t)gi;
        data.getFloat("density", density);
        data.getFloat("friction", friction);
        data.getFloat("restitution", restitution);
        int sensor = 0;
        if (data.getInt("isSensor", sensor) == sfs::noError)
            isSensor = sensor != 0;
    }

    const char* getComponentName() override { return "PhysicsComponent"; }
};
