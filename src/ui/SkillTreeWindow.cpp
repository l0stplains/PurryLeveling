#include "ui/SkillTreeWindow.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

SkillTreeWindow::SkillTreeWindow(GameContext& gameContext)
    : m_gameContext(gameContext),
      m_playerMasteryPoints(30),
      m_initialized(false),
      m_windowSize(ImVec2(0, 0)),  // Will be set to match display size
      m_hoveredSkill(nullptr),
      m_skillTree(*gameContext.GetUnitManager()->GetUnit(gameContext.GetCharacterId())->GetSkillTree()) 
{
    LoadTextures();
}

void SkillTreeWindow::LoadTextures()
{

    // Load base node textures
    if (!m_nodeTexture.loadFromFile("resources/textures/ui/skill_node.png"))
    {
        std::cerr << "Failed to load skill node texture" << std::endl;
    }

    if (!m_nodeActiveTexture.loadFromFile("resources/textures/ui/skill_node_active.png"))
    {
        std::cerr << "Failed to load active skill node texture" << std::endl;
    }

    // Load skill icon textures
    std::vector<std::string> skillNames = this->m_skillTree.getSkillName();
    for (const auto& name : skillNames)
    {
        // Create a space-free version of the name for the filename
        std::string filenameName = name;
        filenameName.erase(std::remove(filenameName.begin(), filenameName.end(), ' '),
                           filenameName.end());

        sf::Texture texture;
        if (!texture.loadFromFile("resources/textures/skills/" + filenameName + ".png"))
        {
            std::cerr << "Failed to load texture for skill: " << name << std::endl;
        }
        else
        {
            m_skillTextures[name] = texture;
        }
    }
}

void SkillTreeWindow::Render()
{
    // Initialize window to match display size
    ImGuiIO& io = ImGui::GetIO();
    if (!m_initialized)
    {
        m_windowSize  = io.DisplaySize;
        m_windowPos   = ImVec2(0, 0);  // Start from top-left corner
        m_initialized = true;
    }

    ImVec2 adjustedSize = ImVec2(m_windowSize.x * 0.78125f, m_windowSize.y * 0.8076171875f);
    ImGui::SetNextWindowPos(ImVec2(137, 117), ImGuiCond_Always);
    ImGui::SetNextWindowSize(adjustedSize, ImGuiCond_Always);

    // Begin window with no padding and no decorations - keep these styles
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.1f, 0.5f)); // Transparent BG

    if (ImGui::Begin("Skill Tree",
                     nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        // Render mastery points indicator in the upper left corner
        RenderMasteryPointsIndicator();
        
        // Rest of the function remains the same
        ImVec2      windowPos   = ImGui::GetWindowPos();
        ImVec2      contentSize = ImGui::GetContentRegionAvail();
        ImDrawList* drawList    = ImGui::GetWindowDrawList();

        // Get available skills
        const Skill* rootSkill = m_skillTree.getSkill().get();
        m_hoveredSkill         = nullptr;

        // Display skill tree
        // ImGui::SetCursorPos(ImVec2(0, 70));  // Position below the header area
        ImGui::BeginChild("SkillTreeView",
                          ImVec2(contentSize.x, contentSize.y-75),  // Reserve space for bottom panel
                          false,
                          ImGuiWindowFlags_NoScrollbar);

        // Set scroll to center initially
        static bool firstRender = true;
        if (firstRender)
        {
            ImGui::SetScrollX(0);
            ImGui::SetScrollY(contentSize.y * 0.3f);  // Position to show the tree better
            firstRender = false;
        }

        // Render skill connections and nodes
        // Rest of rendering code remains the same
        ImDrawList* skillDrawList = ImGui::GetWindowDrawList();
        ImVec2      canvasPos     = ImGui::GetCursorScreenPos();

        // First render connections (lines between nodes)
        if (rootSkill)
        {
            RenderSkillConnections(rootSkill, 0, 0, 0, true);  // Set horizontal layout to true
        }

        // Then render nodes on top of the connections
        if (rootSkill)
        {
            RenderSkillNode(rootSkill, 0, 0, 0, true);  // Set horizontal layout to true
        }

        ImGui::EndChild();

        // Display skill details panel at the bottom
        if (m_hoveredSkill)
        {
            RenderSkillDetails(m_hoveredSkill);
        }
    }

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

void SkillTreeWindow::RenderMasteryPointsIndicator()
{
    // Set custom styles for the mastery points indicator
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f); // Rounded corners
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 6)); // Padding inside the frame
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.2f, 0.7f)); // Dark background
    ImGui::PushStyleColor(ImGuiCol_Border, MASTERY_COLOR); // Mastery color for border
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f); // Thicker border
    
    // Position for the indicator (left corner with some padding)
    ImGui::SetCursorPos(ImVec2(20, 20));
    
    // Create a group to contain the indicator
    ImGui::BeginGroup();
    
    // Display mastery points with an icon/symbol
    ImGui::Text("*"); // Star symbol for mastery
    ImGui::SameLine();
    ImGui::TextColored(MASTERY_COLOR, "Mastery Points: %d", m_playerMasteryPoints);
    
    ImGui::EndGroup();
    
    // Draw a frame around our group
    ImVec2 min_pos = ImGui::GetItemRectMin();
    ImVec2 max_pos = ImGui::GetItemRectMax();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(
        min_pos, 
        max_pos, 
        ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.1f, 0.2f, 0.7f)), 
        12.0f); // Rounded corner
    draw_list->AddRect(
        min_pos, 
        max_pos, 
        ImGui::ColorConvertFloat4ToU32(MASTERY_COLOR), 
        12.0f, // Rounded corner
        0, 
        1.5f); // Border thickness
    
    // Restore original styles
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void SkillTreeWindow::RenderSkillConnections(const Skill* skill, int x, int y, int depth, bool horizontal)
{
    if (!skill)
        return;

    ImDrawList* drawList  = ImGui::GetWindowDrawList();
    ImVec2      parentPos = CalculateNodePosition(x, y, horizontal);

    // Calculate branching layout
    const auto& children    = skill->getChildren();
    int         numChildren = children.size();

    if (numChildren > 0)
    {
        // For horizontal layout (left to right)
        if (horizontal)
        {
            // Calculate height needed for children
            int totalHeight = (numChildren - 1) * 2;  // Vertical spacing between children

            // Calculate starting position for first child
            int startY = y - totalHeight / 2;

            // Draw connections to all children
            for (int i = 0; i < numChildren; ++i)
            {
                const Skill* childSkill = children[i].get();
                int          childX     = x + 2;           // Horizontal spacing (moving right)
                int          childY     = startY + i * 2;  // Vertical position

                ImVec2 childPos = CalculateNodePosition(childX, childY, horizontal);

                // Determine line color based on learned status
                ImVec4 lineColor = LINE_COLOR;
                if (skill->getIsLearned() && childSkill->getIsLearned())
                {
                    lineColor = NODE_ACTIVE_COLOR;
                }
                else if (!childSkill->getIsLearned())
                {
                    lineColor = ImVec4(lineColor.x, lineColor.y, lineColor.z, 0.4f);  // Dimmed
                }

                // Draw the connection line
                drawList->AddLine(ImVec2(parentPos.x + NODE_SIZE / 2, parentPos.y + NODE_SIZE / 2),
                                  ImVec2(childPos.x + NODE_SIZE / 2, childPos.y + NODE_SIZE / 2),
                                  ImGui::ColorConvertFloat4ToU32(lineColor),
                                  LINE_THICKNESS);

                // Recursively draw connections for this child
                RenderSkillConnections(childSkill, childX, childY, depth + 1, horizontal);
            }
        }
        // For vertical layout (top to bottom) - kept for reference
        else
        {
            // Calculate width needed for children
            int totalWidth = (numChildren - 1) * 2;  // Horizontal spacing between children

            // Calculate starting position for first child
            int startX = x - totalWidth / 2;

            // Draw connections to all children
            for (int i = 0; i < numChildren; ++i)
            {
                const Skill* childSkill = children[i].get();
                int          childX     = startX + i * 2;  // Horizontal spacing
                int          childY     = y + 2;           // Vertical spacing

                ImVec2 childPos = CalculateNodePosition(childX, childY, horizontal);

                // Determine line color based on learned status
                ImVec4 lineColor = LINE_COLOR;
                if (skill->getIsLearned() && childSkill->getIsLearned())
                {
                    lineColor = NODE_ACTIVE_COLOR;
                }
                else if (!childSkill->getIsLearned())
                {
                    lineColor = ImVec4(lineColor.x, lineColor.y, lineColor.z, 0.4f);  // Dimmed
                }

                // Draw the connection line
                drawList->AddLine(ImVec2(parentPos.x + NODE_SIZE / 2, parentPos.y + NODE_SIZE / 2),
                                  ImVec2(childPos.x + NODE_SIZE / 2, childPos.y + NODE_SIZE / 2),
                                  ImGui::ColorConvertFloat4ToU32(lineColor),
                                  LINE_THICKNESS);

                // Recursively draw connections for this child
                RenderSkillConnections(childSkill, childX, childY, depth + 1, horizontal);
            }
        }
    }
}

void SkillTreeWindow::RenderSkillNode(const Skill* skill, int x, int y, int depth, bool horizontal)
{
    if (!skill)
        return;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2      nodePos  = CalculateNodePosition(x, y, horizontal);

    // Convert SFML texture to ImGui texture ID
    sf::Texture& nodeTex   = skill->getIsLearned() ? m_nodeActiveTexture : m_nodeTexture;
    ImTextureID  nodeTexID = (ImTextureID)(intptr_t)nodeTex.getNativeHandle();

    // Determine node color based on status
    ImVec4 nodeColor;
    if (skill->getActivated())
    {
        nodeColor = NODE_ACTIVE_COLOR;  // Active
    }
    else if (skill->getIsLearned())
    {
        nodeColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // Learned but not active
    }
    else if (CanLearnSkill(skill))
    {
        nodeColor = ImVec4(0.6f, 0.6f, 0.6f, 0.8f);  // Available
    }
    else
    {
        nodeColor = NODE_UNAVAILABLE_COLOR;  // Unavailable
    }

    // Get non-const pointer for interaction
    Skill* nonConstSkill = const_cast<Skill*>(skill);

    // Draw node
    ImVec2 nodeMax = ImVec2(nodePos.x + NODE_SIZE, nodePos.y + NODE_SIZE);

    // Check for hover and handle scaling
    bool  isHovered = ImGui::IsMouseHoveringRect(nodePos, nodeMax);
    float scale     = isHovered ? NODE_HOVER_SCALE : 1.0f;

    if (isHovered)
    {
        m_hoveredSkill = nonConstSkill;

        // Handle click - learn the skill if possible
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            HandleSkillInteraction(nonConstSkill);
        }

        // Show tooltip
        DrawSkillTooltip(skill);
    }

    // Calculate scaled size and adjusted position
    float  scaledSize       = NODE_SIZE * scale;
    float  offsetForScaling = (scaledSize - NODE_SIZE) / 2;
    ImVec2 scaledPos        = ImVec2(nodePos.x - offsetForScaling, nodePos.y - offsetForScaling);
    ImVec2 scaledMax        = ImVec2(scaledPos.x + scaledSize, scaledPos.y + scaledSize);

    // Draw the node background with proper coloring
    drawList->AddImage(nodeTexID,
                       scaledPos,
                       scaledMax,
                       ImVec2(0, 0),
                       ImVec2(1, 1),
                       ImGui::ColorConvertFloat4ToU32(nodeColor));

    // Draw the skill icon if available
    std::string skillName = skill->getName();
    if (m_skillTextures.find(skillName) != m_skillTextures.end())
    {
        sf::Texture& iconTex   = m_skillTextures[skillName];
        ImTextureID  iconTexID = (ImTextureID)(intptr_t)iconTex.getNativeHandle();

        // Calculate icon size (slightly smaller than node)
        float  iconMargin = NODE_SIZE * 0.15f * scale;
        ImVec2 iconPos    = ImVec2(scaledPos.x + iconMargin, scaledPos.y + iconMargin);
        ImVec2 iconMax    = ImVec2(scaledMax.x - iconMargin, scaledMax.y - iconMargin);

        // Draw the skill icon
        drawList->AddImage(iconTexID, iconPos, iconMax);
    }

    // Calculate branching layout based on orientation
    const auto& children    = skill->getChildren();
    int         numChildren = children.size();

    if (numChildren > 0)
    {
        // For horizontal layout (left to right)
        if (horizontal)
        {
            // Calculate height needed for children
            int totalHeight = (numChildren - 1) * 2;  // Vertical spacing between children

            // Calculate starting position for first child
            int startY = y - totalHeight / 2;

            // Render all children
            for (int i = 0; i < numChildren; ++i)
            {
                const Skill* childSkill = children[i].get();
                int          childX     = x + 2;           // Move right
                int          childY     = startY + i * 2;  // Vertical position

                // Recursively render this child
                RenderSkillNode(childSkill, childX, childY, depth + 1, horizontal);
            }
        }
        // For vertical layout (top to bottom) - kept for reference
        else
        {
            // Calculate width needed for children
            int totalWidth = (numChildren - 1) * 2;  // Horizontal spacing between children

            // Calculate starting position for first child
            int startX = x - totalWidth / 2;

            // Render all children
            for (int i = 0; i < numChildren; ++i)
            {
                const Skill* childSkill = children[i].get();
                int          childX     = startX + i * 2;  // Horizontal spacing
                int          childY     = y + 2;           // Vertical spacing

                // Recursively render this child
                RenderSkillNode(childSkill, childX, childY, depth + 1, horizontal);
            }
        }
    }
}

void SkillTreeWindow::RenderSkillDetails(const Skill* skill)
{
    if (!skill)
        return;

    // Create panel at the bottom of the window
    ImVec2 panelSize = ImVec2(ImGui::GetContentRegionAvail().x, 70);
    ImGui::BeginChild("SkillDetails", panelSize, true);

    // Display skill information in a two-column layout
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, panelSize.x * 0.65f);

    // Column 1: Skill name and description
    ImGui::TextColored(NODE_ACTIVE_COLOR, "%s", skill->getName().c_str());
    ImGui::TextWrapped("Skill description would go here for %s", skill->getName().c_str());

    ImGui::NextColumn();

    // Column 2: Skill stats
    ImGui::TextColored(MANA_COLOR, "Mana Cost: %.1f", skill->getManaCost());
    ImGui::TextColored(MASTERY_COLOR, "Mastery Cost: %d", skill->getMasteryCost());
    ImGui::TextColored(DAMAGE_COLOR, "Damage: %.1f", skill->getDamage());

    // Show effects if any
    const auto& effects = skill->getEffects();
    if (!effects.empty())
    {
        std::string effectsText = "Effects: ";
        for (size_t i = 0; i < effects.size(); ++i)
        {
            if (i > 0)
                effectsText += ", ";
            effectsText += effects[i]->GetName();
        }
        ImGui::TextWrapped("%s", effectsText.c_str());
    }

    ImGui::EndChild();
}

ImVec2 SkillTreeWindow::CalculateNodePosition(int x, int y, bool horizontal)
{
    ImVec2 canvasPos   = ImGui::GetCursorScreenPos();
    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    // For horizontal layout (left to right)
    if (horizontal)
    {
        // Calculate grid positions - center in the available space
        float centerX = contentSize.x * 0.5f;  // Center horizontally
        float centerY = contentSize.y * 0.5f;  // Center vertically

        // Adjust for tree growth to the right by shifting left
        centerX -= (NODE_SIZE + NODE_PADDING * 1.5f) * 3;  // Shift left to accommodate tree growth

        float posX = canvasPos.x + centerX + x * (NODE_SIZE + NODE_PADDING * 1.5f);
        float posY = canvasPos.y + centerY + y * (NODE_SIZE + NODE_PADDING);

        return ImVec2(posX, posY);
    }
    // For vertical layout (top to bottom)
    else
    {
        // Calculate grid positions - center in the available space
        float centerX = contentSize.x * 0.5f;  // Center horizontally
        float centerY = contentSize.y * 0.5f;  // Center vertically

        // Adjust for tree growth downward by shifting up
        centerY -= (NODE_SIZE + NODE_PADDING * 1.5f) * 2;  // Shift up to accommodate tree growth

        float posX = canvasPos.x + centerX + x * (NODE_SIZE + NODE_PADDING);
        float posY = canvasPos.y + centerY + y * (NODE_SIZE + NODE_PADDING * 1.5f);

        return ImVec2(posX, posY);
    }
}

bool SkillTreeWindow::CanLearnSkill(const Skill* skill)
{
    if (!skill)
    {
        return false;
    }
    
    // If already learned, can't learn again
    if (skill->getIsLearned())
    {
        return false;
    }

    // Check mastery points
    if (m_playerMasteryPoints < skill->getMasteryCost())
    {
        return false;
    }

    // Check prerequisites - find the parent skill
    const Skill* rootSkill = m_skillTree.getSkill().get();
    
    // Special case: root skill is always learnable if enough mastery points
    if (skill == rootSkill)
    {
        return true;
    }
    
    // Find the direct parent of this skill
    std::function<const Skill*(const Skill*, const Skill*)> findDirectParent;
    
    findDirectParent = [&findDirectParent](const Skill* current, const Skill* target) -> const Skill* {
        if (!current)
            return nullptr;
            
        // Check direct children first
        for (const auto& child : current->getChildren())
        {
            if (child.get() == target)
            {
                return current;  // This is the direct parent
            }
        }
        
        // Check children recursively
        for (const auto& child : current->getChildren())
        {
            const Skill* foundParent = findDirectParent(child.get(), target);
            if (foundParent)
                return foundParent;
        }
        
        return nullptr;  // Not found in this branch
    };
    
    const Skill* parent = findDirectParent(rootSkill, skill);
    
    // If we found a parent, check if it's learned
    if (parent && parent->getIsLearned())
    {
        return true;
    }
    
    return false;  // No learned parent found
}

void SkillTreeWindow::HandleSkillInteraction(Skill* skill)
{
    if (!skill)
        return;

    // If skill is already learned, we don't allow learning it again
    if (skill->getIsLearned())
    {
        std::cout << skill->getName() << " is already learned." << std::endl; 
        return;
    }
    std::cout << skill->getName() << " clicked" << std::endl;

    // Try to learn the skill
    if (CanLearnSkill(skill))
    {
        // Deduct mastery points (this was missing in the original code)
        m_playerMasteryPoints -= skill->getMasteryCost();
        
        int masteryPoints = m_playerMasteryPoints;
        if (skill->learn(&masteryPoints))
        {
            // Update player's mastery points
            m_playerMasteryPoints = masteryPoints;
            
            // Update the activation status of skills based on the new learning state
            UpdateSkillActivationStatus(m_skillTree.getSkill().get());
            
            std::cout << "Successfully learned " << skill->getName() << std::endl;
        }
        else
        {
            std::cout << "Failed to learn " << skill->getName() << std::endl;
        }
    }
    else
    {
        std::cout << "Cannot learn " << skill->getName() << " - prerequisites not met" << std::endl;
    }
}

// Update skill activation status based on requirements
void SkillTreeWindow::UpdateSkillActivationStatus(Skill* skill)
{
    if (!skill)
        return;

    const auto& children = skill->getChildren();

    // Check if this is a parent skill with children
    if (!children.empty())
    {
        // Count how many children are learned
        int learnedChildren = 0;
        for (const auto& child : children)
        {
            if (child->getIsLearned())
                learnedChildren++;
        }

        // If all direct children are learned, deactivate this parent skill
        if (learnedChildren == children.size() && learnedChildren > 0)
        {
            skill->setActivated(false);
        }
        // Otherwise, activate it if it's learned
        else if (skill->getIsLearned())
        {
            skill->setActivated(true);
        }
    }
    // For skills with no children (leaf skills), keep them activated if learned
    else if (skill->getIsLearned())
    {
        skill->setActivated(true);
    }

    // Recursively update all children
    for (const auto& child : children)
    {
        UpdateSkillActivationStatus(child.get());
    }
}

void SkillTreeWindow::DrawSkillTooltip(const Skill* skill)
{
    ImGui::BeginTooltip();

    ImGui::TextColored(NODE_ACTIVE_COLOR, "%s", skill->getName().c_str());
    ImGui::Separator();

    ImGui::TextColored(MANA_COLOR, "Mana: %.1f", skill->getManaCost());
    ImGui::TextColored(MASTERY_COLOR, "Mastery: %d", skill->getMasteryCost());
    ImGui::TextColored(DAMAGE_COLOR, "Damage: %.1f", skill->getDamage());

    // Show status
    if (skill->getIsLearned())
    {
        if (skill->getActivated())
        {
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Status: Active");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "Status: Learned (inactive)");
        }
    }
    else if (CanLearnSkill(skill))
    {
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 0.8f, 1.0f), "Status: Available to learn");
    }
    else
    {
        ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "Status: Unavailable");
    }

    ImGui::EndTooltip();
}