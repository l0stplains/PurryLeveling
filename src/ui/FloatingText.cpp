#include "ui/FloatingText.hpp"

#include <cmath>
#include <cstdlib>

FloatingText::FloatingText(const std::string&  text,
                           const sf::Vector2f& position,
                           const sf::Color&    color,
                           float               maxLifetime)
    : m_text(text),
      m_position(position),
      m_lifetime(0.0f),
      m_maxLifetime(maxLifetime),
      m_color(color),
      m_scale(1.2f),
      m_alpha(255.0f)
{
    // random X velocity for some variation
    float randomOffset = (std::rand() % 40) - 20.0f;  // -20 to +20
    m_velocity = sf::Vector2f(randomOffset, -60.0f);  // move upward with slight random horizontal
}

bool FloatingText::Update(float dt)
{
    m_lifetime += dt;
    m_position += m_velocity * dt;

    // slow down as it rises
    m_velocity.y *= 0.95f;

    // scale changes: start big, get smaller
    if (m_lifetime < 0.2f)
    {
        // initial grow effect
        m_scale = 1.0f + (m_lifetime / 0.2f) * 0.5f;
    }
    else
    {
        // gradually shrink
        m_scale = 1.5f - ((m_lifetime - 0.2f) / (m_maxLifetime - 0.2f)) * 0.5f;
    }

    // fade out in the second half of lifetime
    if (m_lifetime > m_maxLifetime * 0.5f)
    {
        float fadeProgress = (m_lifetime - m_maxLifetime * 0.5f) / (m_maxLifetime * 0.5f);
        m_alpha            = 255.0f * (1.0f - fadeProgress);
    }

    return !IsExpired();
}

void FloatingText::Draw(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text textDisplay(font);
    textDisplay.setString(m_text);
    textDisplay.setCharacterSize(20);
    textDisplay.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, m_alpha));
    textDisplay.setOutlineColor(sf::Color(0, 0, 0, m_alpha * 0.8f));
    textDisplay.setOutlineThickness(2.0f);
    textDisplay.setScale({m_scale, m_scale});

    // Center the text
    sf::FloatRect textBounds = textDisplay.getLocalBounds();
    textDisplay.setOrigin(textBounds.getCenter());
    textDisplay.setPosition(m_position);

    window.draw(textDisplay);
}

bool FloatingText::IsExpired() const
{
    return m_lifetime >= m_maxLifetime;
}