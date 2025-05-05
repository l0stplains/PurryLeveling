#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

#include "core/GameContext.hpp"

#include "imgui.h"
#include "units/AnimatedUnit.hpp"
#include "units/mobs/Mob.hpp"

/**
 * @brief UI component for displaying information about mobs in the dungeon
 */
class MobInfo
{
public:
    /**
     * @brief Constructor for MobInfo
     *
     * @param gameContext Game context for accessing resources
     * @param windowWidth Width of the UI window
     * @param windowHeight Height of the UI window
     */
    MobInfo(GameContext& gameContext, float windowWidth = 250.0f, float windowHeight = 140.0f);

    /**
     * @brief Render the mob information UI
     *
     * @param mobIds Vector of mob IDs to display information for
     * @param isBossRoom Flag indicating if current chamber is a boss room
     */
    void render(const std::vector<unsigned int>& mobIds, bool isBossRoom = false);

private:
    float        m_windowWidth;
    float        m_windowHeight;
    GameContext& m_gameContext;
    ImVec4       m_titleColor;
    ImVec4       m_statsColor;
    ImVec4       m_valueColor;
    ImVec4       m_backgroundColorDark;
    ImVec4       m_backgroundColorLight;
    ImVec4       m_borderColor;
    ImVec4       m_healthBarColor;
    ImVec4       m_healthBarBgColor;
    ImVec4       m_manaBarColor;
    ImVec4       m_manaBarBgColor;
};