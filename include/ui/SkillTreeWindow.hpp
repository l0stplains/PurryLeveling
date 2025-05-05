#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "core/GameContext.hpp"
#include "skill/SkillTree.hpp"

class SkillTreeWindow
{
private:
    // Constants for rendering
    const float NODE_SIZE         = 50.0f;
    const float NODE_PADDING      = 20.0f;
    const float NODE_HOVER_SCALE  = 1.2f;
    const float LINE_THICKNESS    = 2.0f;
    const ImVec4 LINE_COLOR       = ImVec4(0.6f, 0.6f, 0.8f, 0.7f);
    const ImVec4 NODE_ACTIVE_COLOR = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);
    const ImVec4 NODE_UNAVAILABLE_COLOR = ImVec4(0.4f, 0.4f, 0.4f, 0.6f);
    const ImVec4 MANA_COLOR       = ImVec4(0.2f, 0.5f, 0.9f, 1.0f);
    const ImVec4 MASTERY_COLOR    = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);
    const ImVec4 DAMAGE_COLOR     = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);

    GameContext&              m_gameContext;
    int                       m_playerMasteryPoints;
    bool                      m_initialized;
    ImVec2                    m_windowSize;
    ImVec2                    m_windowPos;
    Skill*                    m_hoveredSkill;
    const SkillTree&                m_skillTree;
    
    // Textures
    sf::Texture m_nodeTexture;
    sf::Texture m_nodeActiveTexture;
    std::unordered_map<std::string, sf::Texture> m_skillTextures;

    void LoadTextures();
    void RenderSkillNode(const Skill* skill, int x, int y, int depth, bool horizontal = false);
    void RenderSkillConnections(const Skill* skill, int x, int y, int depth, bool horizontal = false);
    void RenderSkillDetails(const Skill* skill);
    void RenderMasteryPointsIndicator(); // New function to render mastery points
    
    ImVec2 CalculateNodePosition(int x, int y, bool horizontal = false);
    bool CanLearnSkill(const Skill* skill);
    void HandleSkillInteraction(Skill* skill);
    void UpdateSkillActivationStatus(Skill* skill);
    void DrawSkillTooltip(const Skill* skill);

public:
    SkillTreeWindow(GameContext& gameContext);
    void Render();
};