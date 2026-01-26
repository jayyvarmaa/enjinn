#include "scene.h"

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
}
