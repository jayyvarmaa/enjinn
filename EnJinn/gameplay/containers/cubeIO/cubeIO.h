#pragma once

#include <gl3d.h>
#include <gl2d/gl2d.h>
#include <baseContainer.h>
#include <enjinnSizes.h>
#include <imgui.h>
#include <engineLibraresSupport/engineGL3DSupport.h>
#include <vector>
#include <algorithm>

// CubeIO - A port of the "CubeIO" Unity game
struct CubeIO : public Container
{
    static ContainerStaticInfo containerInfo()
    {
        ContainerStaticInfo info = {};
        info.defaultHeapMemorySize = enjinn::MB(500);
        info.requestImguiFbo = true;
        info.requestImguiIds = 10;
        return info;
    }

    // Engine Components
    gl3d::Renderer3D renderer;
    gl2d::Renderer2D renderer2d;
    gl2d::Font font;

    // Assets
    gl3d::Model cubeModel;
    gl3d::Model planeModel;

    // Game Objects
    // Game Objects
    struct Player {
        gl3d::Entity entity;
        glm::vec3 position{0, 1, 0};
        glm::vec3 velocity{0, 0, 0};
        float forwardForce = 50.0f; // Adjusted for engine Physics
        float sidewaysForce = 30.0f;
        float forwardDrag = 1.5f;
        float sidewaysDrag = 5.0f;
        float width = 1.0f;
    } player;

    // ... (rest of structs) ...

    // ... (in update function) ...

        // --- Inspector ---
        ImGui::PushID(requestedInfo.requestedImguiIds);
        if(ImGui::Begin("CubeIO Inspector"))
        {
            ImGui::Text("Score: %.1f", score);
            
            if(ImGui::CollapsingHeader("Player Physics", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat("Forward Force", &player.forwardForce, 0.5f);
                ImGui::DragFloat("Sideways Force", &player.sidewaysForce, 0.5f);
                ImGui::DragFloat("Forward Drag", &player.forwardDrag, 0.1f);
                ImGui::DragFloat("Sideways Drag", &player.sidewaysDrag, 0.1f);
            }

            if(ImGui::CollapsingHeader("Level Generation", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat("Spawn Interval", &spawnInterval, 0.5f, 5.0f, 50.0f);
                ImGui::DragFloat("Clone Start Z", &spawnStartZ, 1.0f);
            }

            if(ImGui::Button("Reset Game"))
            {
                resetGame();
            }
        }
        ImGui::End();
        ImGui::PopID();

        // --- Physics & Game Loop ---
        if (!gameOver)
        {
            // Forward Movement
            float dt = input.deltaTime;
            // Limit dt for physics stability
            if(dt > 0.05f) dt = 0.05f;

            // Forward force simulation (simply increasing Z here for runner feel)
            // In Unity scripts: rb.AddForce(0, 0, forwardForce * Time.deltaTime);
            // We'll directly integrate velocity for tighter control in this custom engine
            player.velocity.z += player.forwardForce * dt;
            
            // Drag/Friction substitute to keep speed manageable
            player.velocity.z -= player.velocity.z * player.forwardDrag * dt; 

            // Sideways Movement
            if (input.buttons[enjinn::Button::D].held())
            {
                player.velocity.x += player.sidewaysForce * dt;
            }
            if (input.buttons[enjinn::Button::A].held())
            {
                player.velocity.x -= player.sidewaysForce * dt;
            }
            
            // Sideways damping
            player.velocity.x -= player.velocity.x * player.sidewaysDrag * dt; 

            // Integration
            player.position += player.velocity * dt;

            // --- Game Rules ---
            
            // 1. Fall of map
            if (player.position.y < -1.0f)
            {
                gameOver = true;
                restartTimer = 2.0f;
            }

            // 2. Obstacle Collisions & Spawning
            glm::vec3 pSize{0.9f, 0.9f, 0.9f}; // Player slightly smaller than 1 for gameplay feel

            for (size_t i=0; i<obstacles.size(); ++i)
            {
                // Check Collision
                if (checkCollision(player.position, pSize, obstacles[i].position, obstacles[i].size))
                {
                   gameOver = true;
                   restartTimer = 2.0f;
                   // Push player back/up for "crash" effect
                   player.velocity = {0, 5, -5};
                }

                // Cleanup old obstacles
                if (obstacles[i].position.z < player.position.z + cleanupThreshold)
                {
                    renderer.deleteEntity(obstacles[i].entity);
                    obstacles.erase(obstacles.begin() + i);
                    i--;
                }
            }

            // Spawn new obstacles
            if (player.position.z + 100.0f > lastSpawnZ)
            {
                lastSpawnZ += spawnInterval;
                spawnObstacleRow(lastSpawnZ);
            }

            // Update Score
            score = player.position.z;

            // Update Player Entity Transform
            renderer.setEntityTransform(player.entity, gl3d::Transform{player.position});
        }
        else
        {
            // Game Over State
            restartTimer -= input.deltaTime;
            if (restartTimer <= 0)
            {
                resetGame();
            }
        }

        // --- Camera Follow ---
        glm::vec3 camOffset = {0, 5, -8};
        renderer.camera.position = glm::mix(renderer.camera.position, player.position + camOffset, 5.0f * input.deltaTime);
        renderer.camera.viewDirection = glm::normalize((player.position + glm::vec3{0,2,2}) - renderer.camera.position);

        // --- Render ---
        renderer.render(requestedInfo.requestedFBO.fbo);

        // --- UI ---
        renderer2d.flush();
        std::string scoreStr = "Score: " + std::to_string((int)score);
        renderer2d.renderText({10, 10}, scoreStr.c_str(), font, {1,1,1,1}, 2.0f);
        
        if (gameOver)
        {
             renderer2d.renderText({windowState.windowW/2.0f - 100, windowState.windowH/2.0f}, "GAME OVER", font, {1,0,0,1}, 3.0f);
        }

        return true;
    }

    void destruct(RequestedContainerInfo &requestedInfo) override
    {
        // Cleanup handled by destructors
    }
};
