#pragma once

#include "states/State.hpp"
#include "states/WorldState.hpp"

class ChooseCharacterState : public State
{
public:
    explicit ChooseCharacterState(GameContext& context);

    void Init() override;

    State::StateChange ProcessEvent(const sf::Event& event) override;

    State::StateChange Update(const sf::Time& dt) override;

    void Draw(sf::RenderWindow& window) override;

    void RenderUI() override;

    void Exit() override;  // Important for thread cleanup!
private:
    // Helper UI rendering functions
    void RenderCharacterSelectionUI();
    void RenderCharacterInfoWindow();
    void RenderNameInputModal();

    unsigned int m_fighterId;      ///< ID for the Fighter character
    unsigned int m_mageId;         ///< ID for the Mage character
    unsigned int m_assassinId;     ///< ID for the Assassin character
    unsigned int m_necromancerId;  ///< ID for the Necromancer character
    unsigned int m_bersekerId;     ///< ID for the Berseker character

    unsigned int m_selectedCharacterId     = -1;  ///< ID for the selected character
    bool         m_showCharacterInfo       = false;
    bool         m_justOpenedCharacterInfo = false;

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_squareButtonTexture;
    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_exitButton;     ///< Start button
    Button m_confirmButton;  ///< Load game button

    sf::Font    m_font;               ///< Font for UI text
    sf::Font    m_boldFont;           ///< Font for UI text
    sf::Text    m_menuTitle;          ///< Menu title text
    sf::Texture m_backgroundTexture;  ///< Background texture
    sf::Sprite  m_backgroundSprite;   ///< Background sprite

    char m_userNameBuffer[16] = "";  // storage for the input
    bool m_showNameModal      = false;

    State::StateChange m_pendingStateChange;  ///< Pending state change information
};