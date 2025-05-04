#ifndef SKILLTREEWINDOW_HPP
#define SKILLTREEWINDOW_HPP

#include <SFML/Graphics.hpp>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/GameContext.hpp"
#include "parser/PlayerConfigParser.hpp"

#include "imgui-SFML.h"
#include "imgui.h"
#include "skill/Skill.hpp"
#include "skill/SkillTree.hpp"
#include "skill/characterSkill/Mastery1/LifeSteal.hpp"

class SkillTreeWindow
{
public:
    SkillTreeWindow(GameContext& gameContext);
    void Render();
    bool IsExitRequested() const;
    void ResetExitRequest();

private:
    // Constants for visual styling
    static constexpr int   NODE_SIZE        = 64;     // Size of skill node
    static constexpr int   NODE_PADDING     = 20;     // Padding between nodes
    static constexpr int   LINE_THICKNESS   = 2;      // Thickness of connecting lines
    static constexpr int   WINDOW_PADDING   = 16;     // Window padding
    static constexpr float NODE_HOVER_SCALE = 1.15f;  // Scale when hovering

    // Colors
    const ImVec4 NODE_ACTIVE_COLOR      = ImVec4(0.9f, 0.7f, 0.3f, 1.0f);  // Gold
    const ImVec4 NODE_INACTIVE_COLOR    = ImVec4(0.5f, 0.5f, 0.5f, 0.8f);  // Gray
    const ImVec4 NODE_UNAVAILABLE_COLOR = ImVec4(0.3f, 0.3f, 0.3f, 0.5f);  // Dark gray
    const ImVec4 LINE_COLOR             = ImVec4(0.7f, 0.6f, 0.4f, 0.8f);  // Gold line
    const ImVec4 TEXT_COLOR             = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White
    const ImVec4 MANA_COLOR             = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);  // Blue
    const ImVec4 MASTERY_COLOR          = ImVec4(0.8f, 0.3f, 0.3f, 1.0f);  // Red
    const ImVec4 DAMAGE_COLOR           = ImVec4(0.8f, 0.7f, 0.2f, 1.0f);  // Yellow

    // Game context references
    GameContext& m_gameContext;
    const SkillTree&    m_skillTree;
    int          m_playerMasteryPoints;

    // Texture assets
    std::unordered_map<std::string, sf::Texture> m_skillTextures;
    sf::Texture                                  m_nodeTexture;
    sf::Texture                                  m_nodeActiveTexture;
    sf::Texture                                  m_bgTexture;

    // UI state
    bool   m_initialized;
    bool   m_exitRequested;
    ImVec2 m_windowSize;
    ImVec2 m_windowPos;
    Skill* m_hoveredSkill;

    // Helper methods
    void   LoadTextures();
    void   RenderSkillNode(const Skill* skill, int x, int y, int depth, bool horizontal);
    void   RenderSkillConnections(const Skill* skill, int x, int y, int depth, bool horizontal);
    void   RenderSkillDetails(const Skill* skill);
    ImVec2 CalculateNodePosition(int x, int y, bool horizontal);
    void   HandleSkillInteraction(Skill* skill);
    bool   CanLearnSkill(const Skill* skill);
    void   DrawSkillTooltip(const Skill* skill);
    void   UpdateSkillActivationStatus(Skill* skill);
};

#endif  // SKILLTREEWINDOW_HPP