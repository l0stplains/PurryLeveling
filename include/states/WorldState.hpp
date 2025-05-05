#pragma once

#include "skill/characterSkill/Mastery1/Fury.hpp"
#include "states/DialogState.hpp"
#include "states/DungeonState.hpp"
#include "states/InventoryMenuState.hpp"
#include "states/MainMenuState.hpp"
#include "states/ShopState.hpp"
#include "states/State.hpp"
#include "states/SkillTreeState.hpp"

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
    sf::Sound m_backsound;
    DungeonFactory m_dungeonFactory;
    sf::Texture    m_backgroundTexture;  ///< Background texture
    sf::Sprite     m_backgroundSprite;   ///< Background sprite
    sf::Sprite     m_doorEnterArea;      ///< Door enter area sprite

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_squareButtonTexture;
    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_exitButton;       ///< Exit button
    Button m_inventoryButton;  ///< Inventory button
    Button m_skillTreeButton;  ///< Skill tree button

    sf::Font m_font;      ///< Font for UI text
    sf::Font m_boldFont;  ///< Font for UI text

    sf::Vector2f m_lastPosition;  ///< Last player position

    bool m_showPortalEnterModal = false;  ///< Flag for showing portal enter modal
    bool m_showShopEnterModal   = false;  ///< Flag for showing shop enter modal
    bool m_showExitPopup        = false;  ///< Flag to show exit confirmation popup
    bool m_wasInShop            = false;  ///< Flag for showing shop enter modal
    bool m_wasInPortal          = false;  ///< Flag for tracking portal entry

    // Error handling
    bool        m_showErrorPopup = false;  ///< Flag to show error popup
    std::string m_errorMessage;            ///< Error message to display

    // UI rendering methods
    void RenderPortalEnterModal();
    void RenderShopEnterModal(AnimatedUnit* character);
    void RenderErrorModal();
    void RenderExitConfirmationModal();
    void RenderPlayerInfo();

    // Helper methods for modals
    void PushModalStyle(const ImVec4& borderColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    void PopModalStyle();
    bool RenderStyledButton(const char* label, const ImVec2& size, bool isAction = false);

    /**
     * @brief Shows an error message in an ImGui popup
     *
     * @param message The error message to display
     */
    void showError(const std::string& message);

    void generatePortals();

    vector<std::string> generateDungeonRank(int level);

    unsigned int m_currentPortalId = -1;  ///< ID of the current portal

    std::vector<std::unique_ptr<Portal>> m_portals;  ///< List of portals in the world

    State::StateChange m_pendingStateChange;  ///< Pending state change information
};