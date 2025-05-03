#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "core/GameContext.hpp"

#include "imgui.h"
#include "units/AnimatedUnit.hpp"

class BattleUnitInfo
{
public:
    BattleUnitInfo(GameContext& gameContext, float windowWidth = 200.0f, float windowHeight = 400.0f);
    void updateSpriteTexture(const AnimatedUnit& unit);

    void render(const AnimatedUnit& unit);

private:
    float        m_windowWidth;
    float        m_windowHeight;
    GameContext& m_gameContext;
    sf::Texture  m_texture;
};