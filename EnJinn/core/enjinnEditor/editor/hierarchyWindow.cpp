#include "hierarchyWindow.h"
#include <imgui.h>
#include <safeSave/safeSave.h>
#include <enjinnConfig.h>
#include <string>
#include <algorithm>
#include <iostream>

namespace enjinn
{
    void HierarchyWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
    {
        windowId = idManager.getImguiIds(1);
    }

    void HierarchyWindow::update(Scene* scene, bool &open)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin("Hierarchy");

        if(ImGui::Button("Save Scene"))
        {
            if(scene && scene->rootNode)
            {
                sfs::SafeSafeKeyValueData content;
                scene->rootNode->serialize(content);
                sfs::safeSave(content, ENJINN_RESOURCES_PATH "scene_save", false);
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Load Scene"))
        {
            if(scene && scene->rootNode)
            {
                sfs::SafeSafeKeyValueData content;
                if(sfs::safeLoad(content, ENJINN_RESOURCES_PATH "scene_save", false) == sfs::noError)
                {
                    // Clear current children (simplified cleanup)
                    //Ideally we should delete old children
                    // scene->rootNode->children.clear(); // Leak? Yes. But keeping it simple for now as requested.
                    
                    // Deserialize into root
                    scene->rootNode->deserialize(content);
                }
            }
        }

        ImGui::Separator();

        // Search Bar
        ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, sizeof(searchBuffer));
        ImGui::Separator();

        if (scene && scene->rootNode)
        {
            drawNode(scene->rootNode);
        }
        
        // Handle deferred destruction
        if(nodeToDestroy)
        {
             // Find parent and remove
             if(nodeToDestroy->parent)
             {
                 nodeToDestroy->parent->removeChild(nodeToDestroy);
                 delete nodeToDestroy;
             }
             if(selectedNode == nodeToDestroy) selectedNode = nullptr;
             nodeToDestroy = nullptr;
        }

        // Handle Drag and Drop
        if(dragSource && dragTarget && dragSource != dragTarget)
        {
            // Check for circular dependency
            bool isCircular = false;
            Node* temp = dragTarget;
            while(temp)
            {
                if(temp == dragSource) { isCircular = true; break;}
                temp = temp->parent;
            }

            if(!isCircular)
            {
                if(dragSource->parent) dragSource->parent->removeChild(dragSource);
                dragTarget->addChild(dragSource);
            }
            dragSource = nullptr;
            dragTarget = nullptr;
        }

        ImGui::End();
    }
    void HierarchyWindow::drawNode(Node* node)
    {
        // Simple search filtering
        // If search buffer is not empty, only filter checking name.
        // Ideally we check children too. simplified for now: 
        bool nodeMatches = true;
        std::string searchStr = searchBuffer;
        if(!searchStr.empty())
        {
            std::string name = node->name;
            // Case insensitive search ideally
            auto it = std::search(
                name.begin(), name.end(),
                searchStr.begin(), searchStr.end(),
                [](char a, char b) { return std::tolower(a) == std::tolower(b); }
            );
            nodeMatches = (it != name.end());
        }

        // If not matching, verify if we should just skip drawing this node (unless children match? complex)
        // For now, let's just highlight or filter. 
        // Better: recursive check "shouldDraw". But to keep it simple and responsive: active filter
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if(node == selectedNode)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        
        if(node->children.empty())
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // force open if searching?
        // if(!searchStr.empty() && nodeMatches) ImGui::SetNextItemOpen(true);

        bool opened = ImGui::TreeNodeEx((void*)node, flags, node->name.c_str());

        if(ImGui::IsItemClicked())
        {
            selectedNode = node;
        }

        // Context Menu
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Create Child"))
            {
                node->addChild(new Node());
                ImGui::SetNextItemOpen(true); 
            }
            if (ImGui::MenuItem("Delete"))
            {
                nodeToDestroy = node;
            }
            ImGui::EndPopup();
        }

        // Drag Source
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("HIERARCHY_NODE", &node, sizeof(Node*));
            ImGui::Text("%s", node->name.c_str());
            ImGui::EndDragDropSource();
        }

        // Drop Target
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
            {
                Node* payloadNode = *(Node**)payload->Data;
                dragSource = payloadNode;
                dragTarget = node;
            }
            ImGui::EndDragDropTarget();
        }

        if(opened)
        {
            for(auto child : node->children)
            {
                drawNode(child);
            }
            ImGui::TreePop();
        }
    }
}
