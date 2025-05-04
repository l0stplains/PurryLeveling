#include "props/Pointer.hpp"

#include <iostream>

unsigned int Pointer::s_portalIdCounter = 0;

Pointer::Pointer(sf::Texture texture, sf::Vector2f position, sf::Vector2f scale)
    : m_portalId(s_portalIdCounter++),
      m_position(position),
      m_scale(scale),
      m_texture(texture),  // This will copy the texture into m_texture member
      m_sprite(m_texture),
      m_frameSize(32, 32)  // Initialize frame size!
{
    // Load the texture and create the sprite
    sf::FloatRect localBounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin({m_frameSize.x / 2.0f, m_frameSize.y / 2.0f});  // Center origin
    m_sprite.setPosition(m_position);
    m_sprite.setScale(m_scale);

    // Initialize animation as pointer instead of direct object
    m_animation = std::make_unique<Animation>(m_texture,
                                              m_frameSize,  // Now properly initialized
                                              10,           // Number of frames
                                              0.6f,         // Duration for all frames
                                              true,         // Looping
                                              false,        // Not directional
                                              0);           // Default row
}

void Pointer::Draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Pointer::Update(const sf::Time dt)
{
    if (m_animation)  // This is correct now since m_animation is a pointer
    {
        m_animation->Update(dt);
        m_animation->ApplyToSprite(m_sprite);
    }
}

void Pointer::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_sprite.setPosition(m_position);
}

void Pointer::SetScale(const sf::Vector2f& scale)
{
    m_scale = scale;
    m_sprite.setScale(m_scale);
}

unsigned int Pointer::GetId() const
{
    return m_portalId;
}

bool Pointer::isInBounds(const sf::Vector2f& point) const
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    return bounds.contains(point);
}