#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "core/GameContext.hpp"

#include "ui/Button.hpp"

#include "dungeon/Dungeon.hpp"
#include "dungeon/DungeonFactory.hpp"
#include "props/Portal.hpp"
#include "states/Enums.hpp"
#include "states/State.hpp"

// Include ImGuiFileDialog
#include "external/ImGuiFileDialog.h"

class DungeonState : public State
{
public:
    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit DungeonState(GameContext& context, DimensionType dimension, std::string rank);

    void Init() override;

    /**
     * @brief Handle input events
     *
     * @param event The SFML event to handle
     * @param window The SFML window
     * @return State::StateChange The state change information
     */
    State::StateChange ProcessEvent(const sf::Event& event) override;

    State::StateChange Update(const sf::Time& dt) override;

    void Draw(sf::RenderWindow& window) override;

    void RenderUI() override;

    void Exit() override;  // Important for thread cleanup!
    void Pause() override;
    void Resume() override;

private:
    sf::Texture m_backgroundTexture;  ///< Background texture
    sf::Sprite  m_backgroundSprite;   ///< Background sprite
    sf::Sprite  m_chamberExitArea;    ///< Door enter area sprite

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_attackButton;   ///< attack  button
    Button m_useItemButton;  ///< use item button
    Button m_exitButton;     ///< Start button

    sf::Font m_font;      ///< Font for UI text
    sf::Font m_boldFont;  ///< Font for UI text

    bool m_wasInChamberExit = false;

    bool m_showExitPopup = false;  ///< Flag to show exit confirmation popup
    // Dungeon m_dungeon;

    AnimatedUnit*              m_character;
    std::vector<AnimatedUnit*> m_mobs;
    State::StateChange         m_pendingStateChange;  ///< Pending state change information
};