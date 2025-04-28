#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <atomic>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "core/GameContext.hpp"

#include "ui/Button.hpp"

#include "props/Portal.hpp"
#include "states/State.hpp"

// Include ImGuiFileDialog
#include "external/ImGuiFileDialog.h"

class WorldState : public State
{
public:
    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit WorldState(GameContext& context);

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
    sf::Sprite  m_doorEnterArea;      ///< Door enter area sprite

    bool m_showPortalEnterModal = false;  ///< Flag for showing portal enter modal
    bool m_showShopEnterModal   = false;  ///< Flag for showing shop enter modal
    bool m_wasInShop            = false;  ///< Flag for showing shop enter modal
    bool m_wasInPortal          = false;

    unsigned int m_currentPortalId = -1;  ///< ID of the current portal

    std::vector<std::unique_ptr<Portal>> m_portals;  ///< List of portals in the world

    State::StateChange m_pendingStateChange;  ///< Pending state change information
};