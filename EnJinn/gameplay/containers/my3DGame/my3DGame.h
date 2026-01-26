#pragma once

#include <gl3d.h>
#include <gl2d/gl2d.h>
#include <baseContainer.h>
#include <enjinnSizes.h>
#include <imgui.h>
#include <engineLibraresSupport/engineGL3DSupport.h>

// My3DGame - A starter template for 3D games in EnJinn
// Features: First-person camera, basic movement, lighting
struct My3DGame : public Container
{
    static ContainerStaticInfo containerInfo()
    {
        ContainerStaticInfo info = {};
        info.defaultHeapMemorySize = enjinn::MB(500);
        info.requestImguiFbo = true;
        info.requestImguiIds = 10;
        return info;
    }

    // 3D Rendering
    gl3d::Renderer3D renderer;
    
    // Ground plane
    gl3d::Model groundModel;
    gl3d::Entity groundEntity;
    
    // Player cube
    gl3d::Model playerModel;
    gl3d::Entity playerEntity;
    glm::vec3 playerPosition = {0, 1, 0};
    float playerRotation = 0.0f;
    
    // Camera
    bool firstPersonMode = false;
    float cameraDistance = 15.0f;
    float cameraHeight = 10.0f;
    float cameraAngle = 0.0f;
    
    // Game state
    int score = 0;
    
    // Helper to create a colored cube
    gl3d::Model createCubeModel(glm::vec3 color, float metallic = 0.5f)
    {
        static float cubeVertices[] = {
            // Position          Normal           UV
            -1, +1, +1,  0, +1, 0,  0, 0,
            +1, +1, +1,  0, +1, 0,  1, 0,
            +1, +1, -1,  0, +1, 0,  1, 1,
            -1, +1, -1,  0, +1, 0,  0, 1,
            
            -1, +1, -1,  0, 0, -1,  0, 1,
            +1, +1, -1,  0, 0, -1,  1, 1,
            +1, -1, -1,  0, 0, -1,  1, 0,
            -1, -1, -1,  0, 0, -1,  0, 0,
            
            +1, +1, -1,  +1, 0, 0,  1, 0,
            +1, +1, +1,  +1, 0, 0,  1, 1,
            +1, -1, +1,  +1, 0, 0,  0, 1,
            +1, -1, -1,  +1, 0, 0,  0, 0,
            
            -1, +1, +1,  -1, 0, 0,  1, 1,
            -1, +1, -1,  -1, 0, 0,  1, 0,
            -1, -1, -1,  -1, 0, 0,  0, 0,
            -1, -1, +1,  -1, 0, 0,  0, 1,
            
            +1, +1, +1,  0, 0, +1,  1, 1,
            -1, +1, +1,  0, 0, +1,  0, 1,
            -1, -1, +1,  0, 0, +1,  0, 0,
            +1, -1, +1,  0, 0, +1,  1, 0,
            
            +1, -1, -1,  0, -1, 0,  1, 0,
            -1, -1, -1,  0, -1, 0,  0, 0,
            -1, -1, +1,  0, -1, 0,  0, 1,
            +1, -1, +1,  0, -1, 0,  1, 1,
        };
        
        static unsigned int cubeIndices[] = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 22, 21, 20, 23, 22,
        };
        
        auto material = renderer.createMaterial(0, {color, 1}, 0.3f, metallic);
        return renderer.createModelFromData(material, "cube",
            sizeof(cubeVertices)/sizeof(float), cubeVertices,
            sizeof(cubeIndices)/sizeof(unsigned int), cubeIndices);
    }
    
    gl3d::Model createPlaneModel(float size, glm::vec3 color)
    {
        float uv = size / 10.0f;
        std::vector<float> vertices = {
            -size, 0, +size,  0, 1, 0,  0, 0,
            +size, 0, +size,  0, 1, 0,  uv, 0,
            +size, 0, -size,  0, 1, 0,  uv, uv,
            -size, 0, -size,  0, 1, 0,  0, uv,
        };
        std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};
        
        auto material = renderer.createMaterial(0, {color, 1}, 0.8f, 0.0f);
        return renderer.createModelFromData(material, "plane",
            vertices.size(), vertices.data(),
            indices.size(), indices.data());
    }

    bool create(RequestedContainerInfo &requestedInfo, 
                enjinn::StaticString<256> cmdArgs) override
    {
        // Setup file loading callbacks
        renderer.setErrorCallback(&errorCallbackCustom, &requestedInfo);
        renderer.fileOpener.userData = &requestedInfo;
        renderer.fileOpener.readEntireFileBinaryCallback = readEntireFileBinaryCustom;
        renderer.fileOpener.readEntireFileCallback = readEntireFileCustom;
        renderer.fileOpener.fileExistsCallback = defaultFileExistsCustom;
        
        // Initialize renderer
        renderer.init(1, 1, ENJINN_RESOURCES_PATH "BRDFintegrationMap.png", 
                      requestedInfo.requestedFBO.fbo);
        
        // Load skybox
        renderer.skyBox = renderer.loadSkyBox(ENJINN_RESOURCES_PATH "skyBoxes/forest.png");
        
        // Create sun light
        renderer.createDirectionalLight(
            glm::normalize(glm::vec3(-0.3f, -0.8f, -0.5f)), 
            glm::vec3(1.0f, 0.95f, 0.9f)
        );
        
        // Create ground
        groundModel = createPlaneModel(100.0f, {0.3f, 0.5f, 0.2f});
        groundEntity = renderer.createEntity(groundModel, gl3d::Transform{{0, 0, 0}});
        
        // Create player cube
        playerModel = createCubeModel({0.2f, 0.4f, 0.8f});
        playerEntity = renderer.createEntity(playerModel, 
            gl3d::Transform{playerPosition, {}, glm::vec3(0.5f)});
        
        // Add some obstacles
        auto obstacleModel = createCubeModel({0.6f, 0.2f, 0.2f});
        for (int i = 0; i < 10; i++)
        {
            float x = (rand() % 60) - 30.0f;
            float z = (rand() % 60) - 30.0f;
            renderer.createEntity(obstacleModel, 
                gl3d::Transform{{x, 1.0f, z}, {}, glm::vec3(1.0f)});
        }
        
        // Add point lights
        renderer.createPointLight({10, 3, 10}, glm::vec3(1, 0.5f, 0), 20, 2);
        renderer.createPointLight({-10, 3, -10}, glm::vec3(0, 0.5f, 1), 20, 2);
        
        return true;
    }

    bool update(enjinn::Input input, enjinn::WindowState windowState,
                RequestedContainerInfo &requestedInfo) override
    {
        // Setup
        renderer.setErrorCallback(&errorCallbackCustom, &requestedInfo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        renderer.updateWindowMetrics(windowState.windowW, windowState.windowH);
        
        // Toggle first person mode
        if (input.buttons[enjinn::Button::Tab].pressed())
            firstPersonMode = !firstPersonMode;
        
        // Movement
        float speed = 8.0f * input.deltaTime;
        float rotSpeed = 2.0f * input.deltaTime;
        
        if (input.buttons[enjinn::Button::W].held())
        {
            playerPosition.x += sin(playerRotation) * speed;
            playerPosition.z += cos(playerRotation) * speed;
        }
        if (input.buttons[enjinn::Button::S].held())
        {
            playerPosition.x -= sin(playerRotation) * speed;
            playerPosition.z -= cos(playerRotation) * speed;
        }
        if (input.buttons[enjinn::Button::A].held())
            playerRotation += rotSpeed;
        if (input.buttons[enjinn::Button::D].held())
            playerRotation -= rotSpeed;
        
        // Update player entity
        renderer.setEntityTransform(playerEntity, 
            gl3d::Transform{playerPosition, {0, playerRotation, 0}, glm::vec3(0.5f)});
        
        // Camera
        if (firstPersonMode)
        {
            renderer.camera.position = playerPosition + glm::vec3(0, 1.5f, 0);
            renderer.camera.viewDirection = glm::vec3(
                sin(playerRotation), 0, cos(playerRotation));
        }
        else
        {
            renderer.camera.position = playerPosition + 
                glm::vec3(-sin(playerRotation) * cameraDistance, 
                          cameraHeight, 
                          -cos(playerRotation) * cameraDistance);
            renderer.camera.viewDirection = 
                glm::normalize(playerPosition - renderer.camera.position);
        }
        
        // Render
        renderer.render(requestedInfo.requestedFBO.fbo);
        
        // ImGui HUD
        ImGui::SetNextWindowPos({10, 10});
        ImGui::SetNextWindowSize({200, 100});
        ImGui::Begin("Game HUD", nullptr, 
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::Text("WASD - Move/Rotate");
        ImGui::Text("TAB - Toggle Camera");
        ImGui::Text("Position: %.1f, %.1f", playerPosition.x, playerPosition.z);
        ImGui::Text("Mode: %s", firstPersonMode ? "First Person" : "Third Person");
        ImGui::End();
        
        return true;
    }

    void destruct(RequestedContainerInfo &requestedInfo) override
    {
        // Renderer resources are cleaned up automatically by destructor
    }
};
