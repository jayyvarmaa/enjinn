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

    struct Obstacle {
        gl3d::Entity entity;
        glm::vec3 position;
        glm::vec3 size{1, 1, 1};
    };
    std::vector<Obstacle> obstacles;

    // Level Generation
    float lastSpawnZ = 0;
    float spawnStartZ = 40.0f;
    float spawnInterval = 20.0f;
    float cleanupThreshold = -20.0f; // Delete obstacles behind player

    // Game State
    bool gameOver = false;
    float score = 0;
    float restartTimer = 0.0f;

    // Configuration
    int leftKey = enjinn::Button::A;
    int rightKey = enjinn::Button::D;
    bool bindingLeft = false;
    bool bindingRight = false;
    bool paused = false;

    // Visuals
    int currentSkyboxIdx = 0;
    std::vector<const char*> skyboxes = {
        "skyBoxes/forest.png",
        "skyBoxes/desert.png",
        "skyBoxes/lava.png",
        "skyBoxes/mountain.png",
        "skyBoxes/ocean.png",
        "skyBoxes/red.png",
        "skyBoxes/sky.png",
        "skyBoxes/space.png",
        "skyBoxes/Milkyway_small.hdr",
        "skyBoxes/WinterForest_Ref.hdr"
    };

    // --- Helper Models ---
    gl3d::Model createCubeModel(glm::vec3 color, float metallic = 0.0f)
    {
        // Simple cube geometry
        static float v[] = {
            // Pos           Normal    UV
            -1, 1, 1,   0, 1, 0,  0, 0,  1, 1, 1,   0, 1, 0,  1, 0,  1, 1, -1,  0, 1, 0,  1, 1,  -1, 1, -1,  0, 1, 0,  0, 1, // Top
            -1, 1, -1,  0, 0,-1,  0, 1,  1, 1, -1,  0, 0,-1,  1, 1,  1,-1, -1,  0, 0,-1,  1, 0,  -1,-1, -1,  0, 0,-1,  0, 0, // Back
            1, 1, -1,   1, 0, 0,  1, 0,  1, 1, 1,   1, 0, 0,  1, 1,  1,-1, 1,   1, 0, 0,  0, 1,  1,-1, -1,  1, 0, 0,  0, 0, // Right
            -1, 1, 1,  -1, 0, 0,  1, 1, -1, 1, -1, -1, 0, 0,  1, 0, -1,-1, -1, -1, 0, 0,  0, 0, -1,-1, 1,  -1, 0, 0,  0, 1, // Left
            1, 1, 1,   0, 0, 1,   1, 1, -1, 1, 1,   0, 0, 1,  0, 1, -1,-1, 1,   0, 0, 1,  0, 0,  1,-1, 1,   0, 0, 1,  1, 0, // Front
            1,-1, -1,  0,-1, 0,   1, 0, -1,-1, -1,  0,-1, 0,  0, 0, -1,-1, 1,   0,-1, 0,  0, 1,  1,-1, 1,   0,-1, 0,  1, 1, // Bottom
        };
        static unsigned int i[] = {
            0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 12,13,14, 12,14,15, 16,17,18, 16,18,19, 20,22,21, 20,23,22
        };

        auto mat = renderer.createMaterial(0, {color, 1}, 0.5f, metallic);
        return renderer.createModelFromData(mat, "cube", 24*8, v, 36, i);
    }

    gl3d::Model createPlaneModel(float size, glm::vec3 color)
    {
        float uv = size / 2.0f;
        std::vector<float> v = {
            -size, 0, +size,  0, 1, 0,  0, 0,
            +size, 0, +size,  0, 1, 0,  uv, 0,
            +size, 0, -size,  0, 1, 0,  uv, uv,
            -size, 0, -size,  0, 1, 0,  0, uv,
        };
        std::vector<unsigned int> idx = {0, 1, 2, 0, 2, 3};
        
        auto mat = renderer.createMaterial(0, {color, 1}, 0.7f, 0.0f);
        return renderer.createModelFromData(mat, "plane", v.size(), v.data(), idx.size(), idx.data());
    }

    // --- Game Logic ---

    void spawnObstacleRow(float z)
    {
        // X positions corresponding to lanes -5, 0, 5
        static float lanes[] = {-5.0f, 0.0f, 5.0f};
        
        // Randomly choose how many obstacles (1 or 2)
        int count = (rand() % 2) + 1;
        
        std::vector<int> usedLanes;
        
        for(int k=0; k<count; ++k)
        {
            int laneIdx = rand() % 3;
            bool alreadyUsed = false;
            for(int used : usedLanes) if(used == laneIdx) alreadyUsed = true;
            
            if(alreadyUsed) continue;
            usedLanes.push_back(laneIdx);

            Obstacle obs;
            obs.position = {lanes[laneIdx], 1.0f, z + (float)(rand()%10)/5.0f - 1.0f}; // Add slight jitter
            obs.entity = renderer.createEntity(cubeModel, gl3d::Transform{obs.position, {}, obs.size});
            obstacles.push_back(obs);
        }
    }

    void resetGame()
    {
        gameOver = false;
        restartTimer = 0;
        score = 0;
        
        // Reset player
        player.position = {0, 1, 0};
        player.velocity = {0, 0, 0};
        renderer.setEntityTransform(player.entity, gl3d::Transform{player.position});

        // Clear obstacles
        for(auto& obs : obstacles) {
            renderer.deleteEntity(obs.entity);
        }
        obstacles.clear();

        lastSpawnZ = spawnStartZ;
        
        // Initial spawn
        for (float z = spawnStartZ; z < spawnStartZ + 100; z += spawnInterval)
        {
            spawnObstacleRow(z);
            lastSpawnZ = z;
        }
    }

    bool checkCollision(glm::vec3 pPos, glm::vec3 pSize, glm::vec3 oPos, glm::vec3 oSize)
    {
        // AABB Collision
        bool x = std::abs(pPos.x - oPos.x) < (pSize.x + oSize.x) / 2.0f;
        bool y = std::abs(pPos.y - oPos.y) < (pSize.y + oSize.y) / 2.0f;
        bool z = std::abs(pPos.z - oPos.z) < (pSize.z + oSize.z) / 2.0f;
        return x && y && z;
    }

    // --- Lifecycle ---

    bool create(RequestedContainerInfo &requestedInfo, 
                enjinn::StaticString<256> cmdArgs) override
    {
        // Setup renderer
        renderer.setErrorCallback(&errorCallbackCustom, &requestedInfo);
        renderer.fileOpener.userData = &requestedInfo;
        renderer.fileOpener.readEntireFileBinaryCallback = readEntireFileBinaryCustom;
        renderer.fileOpener.readEntireFileCallback = readEntireFileCustom;
        renderer.fileOpener.fileExistsCallback = defaultFileExistsCustom;
        
        renderer.init(1, 1, ENJINN_RESOURCES_PATH "BRDFintegrationMap.png", requestedInfo.requestedFBO.fbo);
        
        // Load default skybox from list
        std::string skyboxPath = std::string(ENJINN_RESOURCES_PATH) + skyboxes[currentSkyboxIdx];
        renderer.skyBox = renderer.loadSkyBox(skyboxPath.c_str());
        
        // UI
        renderer2d.create(requestedInfo.requestedFBO.fbo, 100);
        font.createFromFile(ENJINN_RESOURCES_PATH "arial.ttf"); 

        // Light
        renderer.createDirectionalLight(glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)), glm::vec3(1.2f, 1.1f, 1.0f));

        // Models
        cubeModel = createCubeModel({0.9f, 0.4f, 0.3f}); // Redish player/obstacles
        planeModel = createPlaneModel(1000.0f, {0.8f, 0.8f, 0.8f}); // White ground

        // Ground
        renderer.createEntity(planeModel, gl3d::Transform{{0, 0, 0}});

        // Player Entity
        player.entity = renderer.createEntity(createCubeModel({0.2f, 0.6f, 1.0f}), gl3d::Transform{player.position});

        resetGame();

        return true;
    }

    bool update(enjinn::Input input, enjinn::WindowState windowState,
                RequestedContainerInfo &requestedInfo) override
    {
        renderer.setErrorCallback(&errorCallbackCustom, &requestedInfo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        renderer.updateWindowMetrics(windowState.windowW, windowState.windowH);
        renderer2d.updateWindowMetrics(windowState.windowW, windowState.windowH);

        // Pause Toggle
        if (input.buttons[enjinn::Button::Escape].pressed())
        {
            paused = !paused;
        }

        // Binding Logic
        if (bindingLeft)
        {
            if (input.buttons[enjinn::Button::Escape].pressed())
            {
                bindingLeft = false;
                std::cout << "Binding Left Cancelled.\n";
            }
            else
            {
                for(int i=0; i<enjinn::Button::BUTTONS_COUNT; ++i)
                {
                    if(input.buttons[i].held())
                    {
                        std::cout << "Bind Left: Detected key " << i << " (Held)\n";
                        leftKey = i;
                        bindingLeft = false;
                        break;
                    }
                }
            }
        }
        if (bindingRight)
        {
            if (input.buttons[enjinn::Button::Escape].pressed())
            {
                bindingRight = false;
                std::cout << "Binding Right Cancelled.\n";
            }
            else
            {
                for(int i=0; i<enjinn::Button::BUTTONS_COUNT; ++i)
                {
                    if(input.buttons[i].held())
                    {
                        std::cout << "Bind Right: Detected key " << i << " (Held)\n";
                        rightKey = i;
                        bindingRight = false;
                        break;
                    }
                }
            }
        }

        // --- Inspector ---
        ImGui::PushID(requestedInfo.requestedImguiIds);
        if(ImGui::Begin("CubeIO Inspector"))
        {
            ImGui::Text("Score: %.1f", score);
            if(paused) ImGui::TextColored({1,1,0,1}, "[PAUSED]");
            else ImGui::TextColored({0,1,0,1}, "[RUNNING]");

            if(ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
            {
                std::string lBtnText = bindingLeft ? "Press any key..." : (std::string("Bind Left (Current: ") + std::to_string(leftKey) + ")");
                if(ImGui::Button(lBtnText.c_str())) bindingLeft = true;

                std::string rBtnText = bindingRight ? "Press any key..." : (std::string("Bind Right (Current: ") + std::to_string(rightKey) + ")");
                if(ImGui::Button(rBtnText.c_str())) bindingRight = true;
            }

            if(ImGui::CollapsingHeader("Visuals", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if(ImGui::Combo("Skybox", &currentSkyboxIdx, skyboxes.data(), skyboxes.size()))
                {
                    std::string skyboxPath = std::string(ENJINN_RESOURCES_PATH) + skyboxes[currentSkyboxIdx];
                    renderer.skyBox = renderer.loadSkyBox(skyboxPath.c_str());
                }
            }
            
            if(ImGui::CollapsingHeader("Player Physics"))
            {
                ImGui::DragFloat("Forward Force", &player.forwardForce, 0.5f);
                ImGui::DragFloat("Sideways Force", &player.sidewaysForce, 0.5f);
                ImGui::DragFloat("Forward Drag", &player.forwardDrag, 0.1f);
                ImGui::DragFloat("Sideways Drag", &player.sidewaysDrag, 0.1f);
            }

            if(ImGui::CollapsingHeader("Level Generation"))
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
        if (!gameOver && !paused)
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
            if (input.buttons[rightKey].held())
            {
                player.velocity.x += player.sidewaysForce * dt;
            }
            if (input.buttons[leftKey].held())
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
        else if (gameOver)
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
        else if (paused)
        {
             renderer2d.renderText({windowState.windowW/2.0f - 50, windowState.windowH/2.0f}, "PAUSED", font, {1,1,0,1}, 3.0f);
        }

        return true;
    }

    void destruct(RequestedContainerInfo &requestedInfo) override
    {
        // Cleanup handled by destructors
    }
};
