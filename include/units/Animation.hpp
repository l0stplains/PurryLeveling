#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Animation
{
public:
    Animation(const sf::Texture&  texture,
              const sf::Vector2i& frameSize,
              int                 numFrames,
              float               duration,
              bool                isLooping     = true,
              bool                isDirectional = false,
              int                 defaultRow    = 0);

    bool                Update(const sf::Time& dt);
    void                ApplyToSprite(sf::Sprite& sprite) const;
    bool                IsFinished() const;
    void                Reset();
    void                SetRow(int row);  // For directional spritesheets
    sf::Time            GetDuration() const;
    const sf::Vector2i& GetFrameSize() const;
    bool                IsDirectional() const;
    int                 GetDefaultRow() const;

private:
    const sf::Texture* m_texture;
    sf::Vector2i       m_frameSize;
    int                m_numFrames;
    int                m_currentFrame;
    sf::Time           m_duration;
    sf::Time           m_elapsedTime;
    bool               m_isLooping;
    bool               m_isFinished;
    int                m_row;  // Current row on the spritesheet
    bool               m_isDirectional;
    int                m_defaultRow;
    bool               m_isFlipped;
};