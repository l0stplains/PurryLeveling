// Button.cpp
#include "ui/Button.hpp"

Button::Button(const sf::Texture&     texture,
               const sf::Vector2f&    position,
               const sf::Vector2f&    scale,
               const std::string&     text,
               const sf::Font&        font,
               unsigned int           characterSize,
               sf::Color              color,
               const sf::SoundBuffer& hoverSound,
               const sf::SoundBuffer& clickSound)
    : m_sprite(texture),
      m_baseScale(scale),
      m_text(font, text, characterSize),
      m_hoverSound(hoverSound),
      m_clickSound(clickSound)
{
    // Setup sprite
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);

    // Set origin to center for better positioning and scaling
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
}

bool Button::update(const sf::RenderWindow& window)
{
    if (!m_isActive)
    {
        m_isHovered    = false;
        m_wasHovered   = false;
        m_wasPressed   = false;
        m_mouseWasDown = false;
        return false;
    }

    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    m_wasHovered = m_isHovered;
    m_isHovered  = m_sprite.getGlobalBounds().contains(mousePos);

    if (m_isHovered && !m_wasHovered)
    {
        // m_hoverSound.play();
    }

    updateVisualState();

    bool mouseIsDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Detect button press
    if (m_isHovered && mouseIsDown && !m_mouseWasDown)
    {
        m_wasPressed = true;
    }

    // Detect button release
    if (!mouseIsDown && m_mouseWasDown && m_isHovered && m_wasPressed)
    {
        m_wasPressed = false;
        if (m_onClick)
        {
            m_clickSound.play();
            m_onClick();
        }
        return true;
    }

    // Reset if mouse was released outside
    if (!mouseIsDown && m_mouseWasDown)
    {
        m_wasPressed = false;
    }

    m_mouseWasDown = mouseIsDown;

    return false;
}

void Button::draw(sf::RenderTarget& target) const
{
    target.draw(m_sprite);
    if (!m_text.getString().isEmpty())
    {
        target.draw(m_text);
    }
}

void Button::setOnClickCallback(const std::function<void()>& callback)
{
    m_onClick = callback;
}

void Button::setHoverSound(const sf::Sound& sound)
{
    m_hoverSound = sound;
}

void Button::setClickSound(const sf::Sound& sound)
{
    m_clickSound = sound;
}

void Button::setText(const std::string& text,
                     const sf::Font&    font,
                     unsigned int       characterSize,
                     sf::Color          color)
{
    m_text.setString(text);
    m_text.setFont(font);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(color);

    centerText();
}

void Button::setActive(bool active)
{
    if (m_isActive != active)
    {
        m_isActive = active;
        updateVisualState();
    }
}

bool Button::isActive() const
{
    return m_isActive;
}

void Button::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position);
    centerText();
}

sf::Vector2f Button::getPosition() const
{
    return m_sprite.getPosition();
}

void Button::setScale(const sf::Vector2f& scale)
{
    m_baseScale = scale;
    m_sprite.setScale(m_baseScale);
    centerText();
}

sf::FloatRect Button::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

void Button::centerText()
{
    if (m_text.getString().isEmpty())
    {
        return;
    }

    // Center text on button
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin({textBounds.position.x + textBounds.size.x / 2.f,
                      textBounds.position.y + textBounds.position.y / 2.f});
    m_text.setPosition(m_sprite.getPosition());
}

void Button::updateVisualState()
{
    // Apply hover scale effect
    if (m_isHovered && m_isActive)
    {
        m_sprite.setScale({m_baseScale.x * HOVER_SCALE_FACTOR, m_baseScale.y * HOVER_SCALE_FACTOR});
        m_text.setScale({HOVER_SCALE_FACTOR, HOVER_SCALE_FACTOR});
    }
    else
    {
        m_sprite.setScale(m_baseScale);
        m_text.setScale({1.f, 1.f});
    }

    // Apply active/inactive visual effect
    sf::Color color = m_sprite.getColor();
    if (m_isActive)
    {
        color.a = 255;  // Fully opaque when active
    }
    else
    {
        color.a = INACTIVE_ALPHA;  // Partially transparent when inactive
    }
    m_sprite.setColor(color);

    // Also update text opacity if present
    if (!m_text.getString().isEmpty())
    {
        sf::Color textColor = m_text.getFillColor();
        textColor.a         = color.a;
        m_text.setFillColor(textColor);
    }
}