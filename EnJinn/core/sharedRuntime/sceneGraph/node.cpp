#include "node.h"
#include <safeSave/safeSave.h>
#include "componentFactory.h"
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace enjinn
{

    void Node::addChild(Node *child)
    {
        if(child->parent)
        {
            child->parent->removeChild(child);
        }
        child->parent = this;
        children.push_back(child);
    }

    void Node::removeChild(Node *child)
    {
        for (auto it = children.begin(); it != children.end(); ++it)
        {
            if (*it == child)
            {
                children.erase(it);
                child->parent = nullptr;
                return;
            }
        }
    }

    void Node::updateTransforms()
    {
        glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), localPosition);
        localTransform *= glm::mat4_cast(localRotation);
        localTransform = glm::scale(localTransform, localScale);

        if (parent)
        {
            globalTransform = parent->globalTransform * localTransform;
        }
        else
        {
            globalTransform = localTransform;
        }

        for (auto child : children)
        {
            child->updateTransforms();
        }
    }

    void Node::serialize(sfs::SafeSafeKeyValueData& data)
    {
        data.setString("name", name);
        data.setBool("active", active);
        data.setString("tag", tag);
        data.setInt("layer", layer);
        
        data.setVec3("pos", localPosition.x, localPosition.y, localPosition.z);
        data.setVec4("rot", localRotation.x, localRotation.y, localRotation.z, localRotation.w);
        data.setVec3("scl", localScale.x, localScale.y, localScale.z);

        // Components
        data.setInt("componentCount", (int)components.size());
        for(int i=0; i<components.size(); ++i)
        {
            sfs::SafeSafeKeyValueData compData;
            compData.setString("type", components[i]->getComponentName());
            components[i]->serialize(compData);
            data.setKeyValueData("component_" + std::to_string(i), compData);
        }

        // Children
        data.setInt("childCount", (int)children.size());
        for(int i=0; i<children.size(); ++i)
        {
            sfs::SafeSafeKeyValueData childData;
            children[i]->serialize(childData);
            data.setKeyValueData("child_" + std::to_string(i), childData);
        }
    }

    void Node::deserialize(sfs::SafeSafeKeyValueData& data)
    {
        data.getString("name", name);
        data.getBool("active", active);
        data.getString("tag", tag);
        data.getInt("layer", layer);

        data.getVec3("pos", localPosition.x, localPosition.y, localPosition.z);
        glm::vec4 rot;
        if(data.getVec4("rot", rot.x, rot.y, rot.z, rot.w) == sfs::noError)
        {
            localRotation = glm::quat(rot.w, rot.x, rot.y, rot.z);
        }
        data.getVec3("scl", localScale.x, localScale.y, localScale.z);

        // Components
        int compCount = 0;
        data.getInt("componentCount", compCount);
        for(int i=0; i<compCount; ++i)
        {
            sfs::SafeSafeKeyValueData compData;
            if(data.getKeyValueData("component_" + std::to_string(i), compData) == sfs::noError)
            {
                std::string type;
                if(compData.getString("type", type) == sfs::noError)
                {
                    Component* c = createComponent(type);
                    if(c)
                    {
                        c->node = this;
                        c->deserialize(compData);
                        components.push_back(c);
                        c->onCreate();
                    }
                }
            }
        }

        // Children
        int childCount = 0;
        data.getInt("childCount", childCount);
        for(int i=0; i<childCount; ++i)
        {
            sfs::SafeSafeKeyValueData childData;
            if(data.getKeyValueData("child_" + std::to_string(i), childData) == sfs::noError)
            {
                Node* child = new Node();
                child->deserialize(childData);
                addChild(child);
            }
        }
    }

    Node::~Node()
    {
        for (auto c : components)
        {
            delete c;
        }
        for (auto c : children)
        {
            delete c;
        }
    }

}
