// Button.hpp
#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

/**
 * @class Button
 * @brief A UI button class with hover and click functionality for SFML applications
 *
 * The Button class provides a complete implementation for interactive UI buttons
 * with configurable appearance, hover effects, sound feedback, and click actions.
 * It supports active/inactive states and customizable text display.
 */
class Button
{
public:
    /**
     * @brief Construct a new Button object
     *
     * @param texture The texture to use for the button
     * @param position Initial position of the button
     * @param scale Initial scale of the button (1.0f = original size)
     * @param text Text to display on the button (optional)
     */
    Button(const sf::Texture&     texture,
           const sf::Vector2f&    position,
           const sf::Vector2f&    scale         = {1.0f, 1.0f},
           const std::string&     text          = "",
           const sf::Font&        font          = sf::Font(),
           unsigned int           characterSize = 20,
           sf::Color              color         = sf::Color::White,
           const sf::SoundBuffer& hoverSound    = sf::SoundBuffer(),
           const sf::SoundBuffer& clickSound    = sf::SoundBuffer());

    /**
     * @brief Updates the button state based on mouse position and actions
     *
     * @param window The window used to get mouse position
     * @return true if the button was clicked during this update
     */
    bool update(const sf::RenderWindow& window);

    /**
     * @brief Draws the button to the target window
     *
     * @param target The render target to draw to
     */
    void draw(sf::RenderTarget& target) const;

    /**
     * @brief Set the on-click callback function
     *
     * @param callback Function to call when button is clicked
     */
    void setOnClickCallback(const std::function<void()>& callback);

    /**
     * @brief Set the hover sound to play when button is hovered
     *
     * @param sound Sound to play on hover
     */
    void setHoverSound(const sf::Sound& sound);

    /**
     * @brief Set the click sound to play when button is clicked
     *
     * @param sound Sound to play on click
     */
    void setClickSound(const sf::Sound& sound);

    /**
     * @brief Set the text to display on the button
     *
     * @param text Text string
     * @param font Font to use
     * @param characterSize Size of characters
     * @param color Color of text
     */
    void setText(const std::string& text,
                 const sf::Font&    font,
                 unsigned int       characterSize = 20,
                 sf::Color          color         = sf::Color::White);

    /**
     * @brief Set button active/inactive state
     *
     * @param active true for active button, false for inactive
     */
    void setActive(bool active);

    /**
     * @brief Check if button is currently active
     *
     * @return true if active, false if inactive
     */
    bool isActive() const;

    /**
     * @brief Set button position
     *
     * @param position New position
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Get button position
     *
     * @return Current position
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Set button scale
     *
     * @param scale New scale factor
     */
    void setScale(const sf::Vector2f& scale);

    /**
     * @brief Get button bounds
     *
     * @return Global bounds rectangle
     */
    sf::FloatRect getBounds() const;

private:
    sf::Sprite            m_sprite;      ///< Button sprite
    sf::Text              m_text;        ///< Button text
    sf::Vector2f          m_baseScale;   ///< Original scale before hover effect
    sf::Sound             m_hoverSound;  ///< Sound played on hover
    sf::Sound             m_clickSound;  ///< Sound played on click
    std::function<void()> m_onClick;     ///< Callback for click action

    bool m_isActive     = true;   ///< Button active state
    bool m_isHovered    = false;  ///< Current hover state
    bool m_wasHovered   = false;  ///< Previous hover state for edge detection
    bool m_wasPressed   = false;  ///< Previous pressed state for edge detection
    bool m_mouseWasDown = false;  ///< Previous mouse button state

    static constexpr float HOVER_SCALE_FACTOR = 1.05f;  ///< Scale multiplier when hovered
    static constexpr float INACTIVE_ALPHA     = 128;    ///< Alpha value when inactive (0-255)

    /**
     * @brief Centers text on the button
     */
    void centerText();

    /**
     * @brief Updates button visual state based on active and hover status
     */
    void updateVisualState();
};