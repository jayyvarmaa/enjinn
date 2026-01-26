#pragma once
#include <sceneGraph/component.h>
#include <box2d/box2d.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct GenericPhysicsComponent : public enjinn::Component
{
    b2Body* body = nullptr;
    b2World* world = nullptr;
    glm::vec2 size = {1,1};

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
        
        // Debug info
        auto pos = body->GetPosition();
        ImGui::Text("Body Pos: %.2f, %.2f", pos.x, pos.y);
    }

    void updateFixture()
    {
        if(!body) return;
        b2Fixture* f = body->GetFixtureList();
        // Clear all fixtures (simplified)
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
        boxFixtureDef.density = 1;
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
    }

    void deserialize(sfs::SafeSafeKeyValueData& data) override
    {
        data.getVec2("size", size.x, size.y);
        int type = 0;
        if(data.getInt("bodyType", type) == sfs::noError) // Note: This needs logic to restore body state or defer creation
        {
            // Physics body restoration is complex because it depends on World.
            // For now, we just store the properties.
            // The body creation might need to happen in onCreate() if we had access to world.
        }
    }

    const char* getComponentName() override { return "PhysicsComponent"; }
};
