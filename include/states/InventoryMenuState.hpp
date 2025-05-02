#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "core/GameContext.hpp"

#include "ui/Button.hpp"
#include "ui/InventoryMenu.hpp"

#include "states/State.hpp"

class InventoryMenuState : public State
{
public:
    explicit InventoryMenuState(GameContext& context);

    void Init() override;

    State::StateChange ProcessEvent(const sf::Event& event) override;

    State::StateChange Update(const sf::Time& dt) override;

    void Draw(sf::RenderWindow& window) override;

    void RenderUI() override;

    void Exit() override;  // Important for thread cleanup!
private:
    InventoryMenu m_inventoryMenu;

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_squareButtonTexture;
    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_exitButton;  ///< Start button

    sf::Font    m_font;               ///< Font for UI text
    sf::Texture m_backgroundTexture;  ///< Background texture
    sf::Sprite  m_backgroundSprite;   ///< Background sprite

    // bool m_showNameModal      = false;

    State::StateChange m_pendingStateChange;  ///< Pending state change information
};