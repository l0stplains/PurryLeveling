#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "core/GameContext.hpp"
#include "imgui.h"
#include "units/characters/Character.hpp"
#include "units/AnimatedUnit.hpp"

/**
 * @brief UI component for displaying character information
 */
class CharacterInfo
{
public:
    /**
     * @brief Constructor for CharacterInfoUI
     * 
     * @param gameContext Game context for accessing resources
     * @param windowWidth Width of the UI window
     * @param windowHeight Height of the UI window
     */
    CharacterInfo(GameContext& gameContext, float windowWidth = 250.0f, float windowHeight = 160.0f);

    /**
     * @brief Render the character information UI
     * 
     * @param character The character whose info will be displayed
     */
    void render(const Character& character, const AnimatedUnit& animatedUnit);

    /**
     * @brief Get the character type as a string
     * 
     * @param character The character
     * @return std::string The character type (Assassin, Fighter, etc.)
     */
    std::string getCharacterType(const Character& character) const;

private:
    float m_windowWidth;
    float m_windowHeight;
    GameContext& m_gameContext;
    ImVec4 m_titleColor;
    ImVec4 m_statsColor;
    ImVec4 m_valueColor;
    ImVec4 m_backgroundColorDark;
    ImVec4 m_backgroundColorLight;
    ImVec4 m_borderColor;
};