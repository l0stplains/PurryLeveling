#pragma once

#include <SFML/Graphics.hpp>

#include <optional>

#include "dungeon/Dungeon.hpp"
#include "states/Enums.hpp"
#include "units/Animation.hpp"

class Portal
{
public:
    Portal(sf::Texture   texture,
           sf::Vector2f  position,
           sf::Vector2f  scale,
           Dungeon       dungeon,
           DimensionType dimension);
    ~Portal() = default;

    void Draw(sf::RenderWindow& window);
    void Update(const sf::Time dt);

    Dungeon&      GetDungeon();
    DimensionType GetDimensionType();

    void SetPosition(const sf::Vector2f& position);
    void SetScale(const sf::Vector2f& scale);

    unsigned int GetId() const;

    bool isInBounds(const sf::Vector2f& point) const;

private:
    Dungeon       m_dungeon;
    DimensionType m_dimensionType;
    unsigned int  m_portalId;
    sf::Vector2f  m_position;
    sf::Vector2f  m_scale;

    sf::Vector2i m_frameSize = {64, 64};  //  each frame is 64x64 pixels

    sf::Texture                m_texture;  // Stored as value, not reference
    sf::Sprite                 m_sprite;
    std::unique_ptr<Animation> m_animation;  // Store as pointer

    // TODO: add dungeon
    // Dungeon* m_dungeon;

    static unsigned int s_portalIdCounter;
};