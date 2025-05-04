#pragma once
#include "ui/Button.hpp"
#include "ui/SkillTreeWindow.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "core/GameContext.hpp"
#include "states/State.hpp"

class SkillTreeState : public State
{
public:
    explicit SkillTreeState(GameContext& context);

    void               Init() override;
    State::StateChange ProcessEvent(const sf::Event& event) override;
    State::StateChange Update(const sf::Time& dt) override;
    void               Draw(sf::RenderWindow& window) override;
    void               RenderUI() override;
    void               Exit() override;

private:
    SkillTreeWindow    m_skillTreeWindow;

    sf::Sound          m_buttonHoverSound;    ///< Button hover sound
    sf::Sound          m_buttonClickSound;    ///< Button click sound
    
    Button m_exitButton;  ///< Start button    
    
    sf::Texture m_squareButtonTexture;
    sf::Texture m_buttonTexture;
    
    sf::Texture m_backgroundTexture;  ///< Background texture
    sf::Sprite  m_backgroundSprite;   ///< Background sprite
    sf::Font m_font;  ///< Font for UI text
    
    State::StateChange m_pendingStateChange;  // Added pending state change
};