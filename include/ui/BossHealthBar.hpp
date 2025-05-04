#pragma once

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <memory>
#include <string>

#include "units/Unit.hpp"

class BossHealthBar
{
public:
    BossHealthBar(float height = 60.0f);

    void show();
    void hide();
    bool isVisible() const;

    void render(const Unit& boss);

    void setHeight(float height);

private:
    float m_height;
    bool  m_isVisible;
    // sf::Font m_font; // You could use a custom font if desired for the boss name
};