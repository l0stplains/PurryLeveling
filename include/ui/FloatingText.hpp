#pragma once

#include <SFML/Graphics.hpp>

#include <string>

/**
 * @brief Represents a floating text element that appears and animates above units.
 */
class FloatingText
{
public:
    /**
     * @brief Constructs a new floating text.
     * @param text The text to display
     * @param position Initial position
     * @param color Text color
     * @param maxLifetime Duration before disappearing
     */
    FloatingText(const std::string&  text,
                 const sf::Vector2f& position,
                 const sf::Color&    color       = sf::Color::Red,
                 float               maxLifetime = 1.5f);

    /**
     * @brief Updates the floating text animation.
     * @param dt Delta time in seconds
     * @return True if the text is still active, false if it should be removed
     */
    bool Update(float dt);

    /**
     * @brief Draws the floating text to the given render window.
     * @param window The window to draw to
     * @param font The font to use for rendering
     */
    void Draw(sf::RenderWindow& window, const sf::Font& font);

    /**
     * @brief Checks if the text has expired and should be removed.
     * @return True if expired, false otherwise
     */
    bool IsExpired() const;

private:
    std::string  m_text;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float        m_lifetime;
    float        m_maxLifetime;
    sf::Color    m_color;
    float        m_scale;
    float        m_alpha;
};
