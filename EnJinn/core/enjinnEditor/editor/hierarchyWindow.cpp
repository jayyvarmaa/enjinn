#include "hierarchyWindow.h"
#include <imgui.h>
#include <safeSave/safeSave.h>
#include <enjinnConfig.h>
#include <sceneGraph/componentFactory.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <cstring>

namespace enjinn
{
    void HierarchyWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
    {
        windowId = idManager.getImguiIds(1);
    }
    
    bool HierarchyWindow::isNodeVisible(Node* node) const
    {
        auto it = nodeVisibility.find(node);
        return it != nodeVisibility.end() ? it->second : true;
    }
    
    bool HierarchyWindow::isNodeLocked(Node* node) const
    {
        auto it = nodeLocked.find(node);
        return it != nodeLocked.end() ? it->second : false;
    }
    
    void HierarchyWindow::setNodeVisible(Node* node, bool visible)
    {
        nodeVisibility[node] = visible;
    }
    
    void HierarchyWindow::setNodeLocked(Node* node, bool locked)
    {
        nodeLocked[node] = locked;
    }
    
    const char* HierarchyWindow::getNodeIcon(Node* node)
    {
        if (!node) return ICON_FK_CUBE;
        
        // Check components to determine icon
        for (auto* comp : node->components)
        {
            std::string compName = comp->getComponentName();
            if (compName.find("Camera") != std::string::npos)
                return ICON_FK_VIDEO_CAMERA;
            if (compName.find("Light") != std::string::npos)
                return ICON_FK_LIGHTBULB_O;
            if (compName.find("Sprite") != std::string::npos)
                return ICON_FK_PICTURE_O;
            if (compName.find("Physics") != std::string::npos)
                return ICON_FK_CIRCLE_O;
        }
        
        // Check children count
        if (node->children.empty())
            return ICON_FK_CUBE;
        else
            return ICON_FK_FOLDER;
    }

    void HierarchyWindow::update(Scene* scene, bool &open)
    {
        ImGui::SetNextWindowSize(ImVec2(320, 600), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
        
        if (!ImGui::Begin(ICON_NAME, &open))
        {
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        // Top toolbar
        if(ImGui::Button(ICON_FK_PLUS " Create"))
        {
            ImGui::OpenPopup("CreateNodePopup");
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Create new GameObject");
            
        if (ImGui::BeginPopup("CreateNodePopup"))
        {
            if (ImGui::MenuItem(ICON_FK_CUBE " Empty GameObject"))
            {
                if (scene && scene->rootNode)
                {
                    Node* newNode = new Node();
                    newNode->name = "GameObject";
                    scene->rootNode->addChild(newNode);
                    selectedNode = newNode;
                }
            }
            if (ImGui::MenuItem(ICON_FK_FOLDER " Empty Parent"))
            {
                if (scene && scene->rootNode)
                {
                    Node* newNode = new Node();
                    newNode->name = "EmptyParent";
                    scene->rootNode->addChild(newNode);
                    selectedNode = newNode;
                }
            }
            ImGui::EndPopup();
        }
        
        ImGui::SameLine();
        
        if(ImGui::Button(ICON_FK_FLOPPY_O " Save"))
        {
            if(scene && scene->rootNode)
            {
                sfs::SafeSafeKeyValueData content;
                scene->rootNode->serialize(content);
                sfs::safeSave(content, ENJINN_RESOURCES_PATH "scene_save", false);
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Save Scene (Ctrl+S)");
            
        ImGui::SameLine();
        
        if(ImGui::Button(ICON_FK_FOLDER_OPEN " Load"))
        {
            if(scene && scene->rootNode)
            {
                sfs::SafeSafeKeyValueData content;
                if(sfs::safeLoad(content, ENJINN_RESOURCES_PATH "scene_save", false) == sfs::noError)
                {
                    scene->rootNode->deserialize(content);
                    selectedNode = nullptr;
                }
            }
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Load Scene (Ctrl+O)");

        ImGui::Separator();

        // Search Bar with icon
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputTextWithHint("##Search", ICON_FK_SEARCH " Search...", searchBuffer, sizeof(searchBuffer)))
        {
            // Filter as you type
        }
        
        ImGui::Separator();
        
        // Scene tree
        ImGui::BeginChild("SceneTree", ImVec2(0, 0), true);

        if (scene && scene->rootNode)
        {
            drawNode(scene->rootNode, 0);
        }
        else
        {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No scene loaded");
        }
        
        ImGui::EndChild();
        
        // Handle deferred destruction
        if(nodeToDestroy)
        {
             // Clear selectedNode BEFORE deleting to avoid use-after-free
             if(selectedNode == nodeToDestroy) selectedNode = nullptr;
             if(nodeToDestroy->parent)
             {
                 nodeToDestroy->parent->removeChild(nodeToDestroy);
                 delete nodeToDestroy;
             }
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
        ImGui::PopStyleVar();
    }
    void HierarchyWindow::drawNode(Node* node, int depth)
    {
        // Simple search filtering
        bool nodeMatches = true;
        if (searchBuffer[0] != '\0')
        {
            std::string searchStr = searchBuffer;
            std::string nodeName = node->name;
            std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
            std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
            nodeMatches = nodeName.find(searchStr) != std::string::npos;
        }
        
        if (!nodeMatches && !node->children.empty())
        {
            // Check if any child matches
            bool anyChildMatches = false;
            for (auto* child : node->children)
            {
                std::string childName = child->name;
                std::string searchStr = searchBuffer;
                std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
                std::transform(childName.begin(), childName.end(), childName.begin(), ::tolower);
                if (childName.find(searchStr) != std::string::npos)
                {
                    anyChildMatches = true;
                    break;
                }
            }
            if (!anyChildMatches) return;
        }

        ImGui::PushID(node);
        
        // Indentation
        if (depth > 0)
            ImGui::Indent(16.0f * depth);
        
        // Node flags
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | 
                                 ImGuiTreeNodeFlags_SpanAvailWidth;
        
        if (node->children.empty())
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            
        if (selectedNode == node)
            flags |= ImGuiTreeNodeFlags_Selected;
        
        // Check if expanded
        bool isExpanded = nodeExpanded[node];
        if (isExpanded)
            ImGui::SetNextItemOpen(true);
        
        // Visibility icon (eye)
        bool visible = isNodeVisible(node);
        const char* eyeIcon = visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH;
        
        ImGui::PushStyleColor(ImGuiCol_Text, visible ? ImVec4(1,1,1,1) : ImVec4(0.5f,0.5f,0.5f,1));
        if (ImGui::SmallButton(eyeIcon))
        {
            setNodeVisible(node, !visible);
        }
        ImGui::PopStyleColor();
        
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(visible ? "Hide" : "Show");
        
        ImGui::SameLine();
        
        // Lock icon
        bool locked = isNodeLocked(node);
        const char* lockIcon = locked ? ICON_FK_LOCK : ICON_FK_UNLOCK;
        
        if (ImGui::SmallButton(lockIcon))
        {
            setNodeLocked(node, !locked);
        }
        
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(locked ? "Unlock" : "Lock");
        
        ImGui::SameLine();
        
        // Node icon and name
        const char* icon = getNodeIcon(node);
        
        // If this node is being renamed, show input text instead of tree label
        if (renamingNode == node)
        {
            std::string tempLabel = std::string(icon) + " ##rename";
            bool node_open_rename = ImGui::TreeNodeEx(tempLabel.c_str(), flags);
            
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##RenameInput", renameBuffer, sizeof(renameBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
            {
                node->name = renameBuffer;
                renamingNode = nullptr;
            }
            // Cancel on Escape or click elsewhere
            if (ImGui::IsKeyPressed(ImGuiKey_Escape) || 
                (!ImGui::IsItemActive() && !ImGui::IsItemFocused() && ImGui::IsMouseClicked(0)))
            {
                renamingNode = nullptr;
            }
            
            // Still need to draw children
            if (node_open_rename && !node->children.empty())
            {
                nodeExpanded[node] = true;
                for (auto* child : node->children)
                    drawNode(child, depth + 1);
                if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
                    ImGui::TreePop();
            }
            else
            {
                nodeExpanded[node] = false;
            }
            
            if (depth > 0)
                ImGui::Unindent(16.0f * depth);
            ImGui::PopID();
            return;
        }
        
        std::string label = std::string(icon) + " " + node->name;
        
        bool node_open = ImGui::TreeNodeEx(label.c_str(), flags);
        
        // Handle selection
        if (ImGui::IsItemClicked())
        {
            if (!ImGui::GetIO().KeyCtrl)
            {
                selectedNodes.clear();
                selectedNode = node;
                selectedNodes.push_back(node);
            }
            else
            {
                // Multi-select with Ctrl
                auto it = std::find(selectedNodes.begin(), selectedNodes.end(), node);
                if (it != selectedNodes.end())
                {
                    selectedNodes.erase(it);
                    if (selectedNode == node)
                        selectedNode = selectedNodes.empty() ? nullptr : selectedNodes[0];
                }
                else
                {
                    selectedNodes.push_back(node);
                    selectedNode = node;
                }
            }
        }
        
        // Handle right-click context menu
        if (ImGui::IsItemClicked(1))
        {
            contextMenuNode = node;
            ImGui::OpenPopup("NodeContextMenu");
        }
        
        // Context menu
        if (contextMenuNode == node && ImGui::BeginPopup("NodeContextMenu"))
        {
            drawContextMenu(node);
            ImGui::EndPopup();
        }
        
        // Drag and drop source
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENE_NODE", &node, sizeof(Node*));
            ImGui::Text("%s %s", icon, node->name.c_str());
            dragSource = node;
            ImGui::EndDragDropSource();
        }
        
        // Drag and drop target
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_NODE"))
            {
                dragTarget = node;
            }
            ImGui::EndDragDropTarget();
        }
        
        // Draw children
        if (node_open && !node->children.empty())
        {
            nodeExpanded[node] = true;
            
            for (auto* child : node->children)
            {
                drawNode(child, depth + 1);
            }
            
            if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
                ImGui::TreePop();
        }
        else
        {
            nodeExpanded[node] = false;
        }
        
        if (depth > 0)
            ImGui::Unindent(16.0f * depth);
        
        ImGui::PopID();
    }
    
    void HierarchyWindow::drawContextMenu(Node* node)
    {
        if (ImGui::MenuItem(ICON_FK_PLUS " Create Child"))
        {
            Node* newNode = new Node();
            newNode->name = "GameObject";
            node->addChild(newNode);
            selectedNode = newNode;
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem(ICON_FK_CLONE " Copy", "Ctrl+C"))
        {
            copiedNode = node;
        }
        
        if (ImGui::MenuItem(ICON_FK_CLIPBOARD " Paste", "Ctrl+V", false, copiedNode != nullptr))
        {
            if (copiedNode)
            {
                // Deep copy: clone transform, components, and children recursively
                std::function<Node*(Node*)> deepCopy = [&](Node* src) -> Node*
                {
                    Node* dst = new Node();
                    dst->name = src->name;
                    dst->active = src->active;
                    dst->tag = src->tag;
                    dst->layer = src->layer;
                    dst->localPosition = src->localPosition;
                    dst->localRotation = src->localRotation;
                    dst->localScale = src->localScale;
                    
                    // Clone components via serialize/deserialize
                    for (auto* comp : src->components)
                    {
                        Component* copy = enjinn::createComponent(comp->getComponentName());
                        if (copy)
                        {
                            sfs::SafeSafeKeyValueData tempData;
                            comp->serialize(tempData);
                            copy->deserialize(tempData);
                            copy->node = dst;
                            dst->components.push_back(copy);
                        }
                    }
                    
                    // Recurse into children
                    for (auto* child : src->children)
                    {
                        Node* childCopy = deepCopy(child);
                        dst->addChild(childCopy);
                    }
                    
                    return dst;
                };
                
                Node* newNode = deepCopy(copiedNode);
                newNode->name += " (Copy)";
                node->addChild(newNode);
                selectedNode = newNode;
            }
        }
        
        if (ImGui::MenuItem(ICON_FK_CLONE " Duplicate", "Ctrl+D"))
        {
            // Deep duplicate: clone transform, components, and children recursively
            std::function<Node*(Node*)> deepCopy = [&](Node* src) -> Node*
            {
                Node* dst = new Node();
                dst->name = src->name;
                dst->active = src->active;
                dst->tag = src->tag;
                dst->layer = src->layer;
                dst->localPosition = src->localPosition;
                dst->localRotation = src->localRotation;
                dst->localScale = src->localScale;
                
                for (auto* comp : src->components)
                {
                    Component* copy = enjinn::createComponent(comp->getComponentName());
                    if (copy)
                    {
                        sfs::SafeSafeKeyValueData tempData;
                        comp->serialize(tempData);
                        copy->deserialize(tempData);
                        copy->node = dst;
                        dst->components.push_back(copy);
                    }
                }
                
                for (auto* child : src->children)
                {
                    Node* childCopy = deepCopy(child);
                    dst->addChild(childCopy);
                }
                
                return dst;
            };
            
            Node* newNode = deepCopy(node);
            newNode->name += " (Copy)";
            
            if (node->parent)
                node->parent->addChild(newNode);
                
            selectedNode = newNode;
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem(ICON_FK_PENCIL " Rename", "F2"))
        {
            renamingNode = node;
            strncpy(renameBuffer, node->name.c_str(), sizeof(renameBuffer) - 1);
            renameBuffer[sizeof(renameBuffer) - 1] = '\0';
        }
        
        ImGui::Separator();
        
        if (ImGui::MenuItem(ICON_FK_TRASH " Delete", "Delete"))
        {
            nodeToDestroy = node;
        }
    }
}
