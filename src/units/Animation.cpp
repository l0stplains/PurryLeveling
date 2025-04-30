#include "units/Animation.hpp"

#include <SFML/System/Time.hpp>

#include <cmath>  // For fmod

Animation::Animation(const sf::Texture&  texture,
                     const sf::Vector2i& frameSize,
                     int                 numFrames,
                     float               duration,
                     bool                isLooping,
                     bool                isDirectional,
                     int                 defaultRow)
    : m_texture(&texture),
      m_frameSize(frameSize),
      m_numFrames(numFrames > 0 ? numFrames : 1),  // Ensure at least 1 frame
      m_currentFrame(0),
      m_duration(sf::seconds(duration > 0.f ? duration : 0.1f)),
      m_elapsedTime(sf::Time::Zero),
      m_isLooping(isLooping),
      m_isFinished(false),
      m_row(0),
      m_isDirectional(isDirectional),
      m_defaultRow(defaultRow)
{}

bool Animation::Update(const sf::Time& dt)
{
    if (m_isFinished && !m_isLooping)
    {
        return true;  // Already finished, non-looping
    }

    m_elapsedTime += dt;
    sf::Time timePerFrame = m_duration / static_cast<float>(m_numFrames);
    if (timePerFrame <= sf::Time::Zero)
        timePerFrame = sf::seconds(0.1f);

    bool animationJustFinished = false;

    // Process frame updates based on elapsed time
    while (m_elapsedTime >= timePerFrame)
    {
        m_elapsedTime -= timePerFrame;
        m_currentFrame++;

        if (m_currentFrame >= m_numFrames)
        {
            if (m_isLooping)
            {
                m_currentFrame = 0;
                m_isFinished   = false;  // Reset finished state if looping
            }
            else
            {
                m_currentFrame        = m_numFrames - 1;  // Stay on the last frame
                m_isFinished          = true;
                animationJustFinished = true;
                // Don't break here if dt caused multiple frame skips past the end
            }
        }
        else
        {
            m_isFinished = false;  // Not finished if we just advanced to a valid frame
        }
    }

    return animationJustFinished;  // Return true ONLY on the frame it finishes
}

void Animation::ApplyToSprite(sf::Sprite& sprite) const
{
    if (!m_texture)
        return;

    int columns  = m_texture->getSize().x / m_frameSize.x;
    int textureX = (m_currentFrame % columns) * m_frameSize.x;
    int textureY = m_row * m_frameSize.y;

    sprite.setTexture(*m_texture, true);  // Update texture just in case
    sprite.setTextureRect(sf::IntRect({textureX, textureY}, m_frameSize));
}

bool Animation::IsFinished() const
{
    return m_isFinished;
}

void Animation::Reset()
{
    m_currentFrame = 0;
    m_elapsedTime  = sf::Time::Zero;
    m_isFinished   = false;
}

void Animation::SetRow(int row)
{
    // Add bounds checking if necessary based on texture dimensions
    m_row = row;
}

sf::Time Animation::GetDuration() const
{
    return m_duration;
}

const sf::Vector2i& Animation::GetFrameSize() const
{
    return m_frameSize;
}

bool Animation::IsDirectional() const
{
    return m_isDirectional;
}

int Animation::GetDefaultRow() const
{
    return m_defaultRow;
}