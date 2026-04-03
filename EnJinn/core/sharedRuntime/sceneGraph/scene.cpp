#include "scene.h"
#include <safeSave/safeSave.h>

namespace enjinn
{
    Scene::Scene()
    {
        rootNode = new Node();
        rootNode->name = "Root";
    }

    Scene::~Scene()
    {
        delete rootNode;
    }

    void updateNode(Node* node, float deltaTime)
    {
        for(auto c : node->components)
        {
            c->onUpdate(deltaTime);
        }
        for(auto c : node->children)
        {
            updateNode(c, deltaTime);
        }
    }

    void Scene::update(float deltaTime)
    {
        if(rootNode)
        {
            rootNode->updateTransforms();
            updateNode(rootNode, deltaTime);
        }
    }

    void renderNode(Node* node)
    {
        for(auto c : node->components)
        {
            c->onRender();
        }
        for(auto c : node->children)
        {
            renderNode(c);
        }
    }

    void Scene::render()
    {
        if(rootNode)
        {
            renderNode(rootNode);
        }
    }

    bool Scene::saveScene(const char* filepath)
    {
        if (!rootNode || !filepath) return false;
        
        sfs::SafeSafeKeyValueData data;
        data.setString("engineVersion", "EnJinn_1.0");
        rootNode->serialize(data);
        
        auto err = sfs::safeSave(data, filepath, false);
        if (err == sfs::noError)
        {
            scenePath = filepath;
            return true;
        }
        return false;
    }

    bool Scene::loadScene(const char* filepath)
    {
        if (!filepath) return false;
        
        sfs::SafeSafeKeyValueData data;
        auto err = sfs::safeLoad(data, filepath, false);
        if (err != sfs::noError) return false;
        
        // Clear existing scene
        clear();
        
        // Rebuild from data
        rootNode = new Node();
        rootNode->deserialize(data);
        scenePath = filepath;
        return true;
    }

    void Scene::clear()
    {
        delete rootNode;
        rootNode = new Node();
        rootNode->name = "Root";
        scenePath.clear();
    }
}
