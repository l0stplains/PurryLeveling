#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "core/GameContext.hpp"

#include "imgui.h"
#include "dungeon/Dungeon.hpp"

class QuestInfo
{
public:
    QuestInfo(GameContext& gameContext, float windowWidth = 40.0f, float windowHeight = 35.0f);

    void render(const Dungeon& dungeon);

private:
    float        m_windowWidth;
    float        m_windowHeight;
    GameContext& m_gameContext;
};