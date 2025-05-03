#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "core/GameContext.hpp"

#include "ui/Button.hpp"

#include "props/Portal.hpp"
#include "save/PlayerConfigSaver.hpp"
#include "states/State.hpp"

// Include ImGuiFileDialog
#include "dungeon/DungeonFactory.hpp"
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
    DungeonFactory m_dungeonFactory;
    sf::Texture    m_backgroundTexture;  ///< Background texture
    sf::Sprite     m_backgroundSprite;   ///< Background sprite
    sf::Sprite     m_doorEnterArea;      ///< Door enter area sprite

    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound

    sf::Texture m_squareButtonTexture;
    sf::Texture m_buttonTexture;  ///< Button texture

    Button m_exitButton;       ///< Start button
    Button m_inventoryButton;  ///< Start button
    Button m_skillTreeButton;  ///< Start button

    sf::Font m_font;      ///< Font for UI text
    sf::Font m_boldFont;  ///< Font for UI text

    sf::Vector2f m_lastPosition;  ///< Last player position

    bool m_showPortalEnterModal = false;  ///< Flag for showing portal enter modal
    bool m_showShopEnterModal   = false;  ///< Flag for showing shop enter modal
    bool m_showExitPopup        = false;  ///< Flag to show exit confirmation popup
    bool m_showSaveAsPopup      = false;  ///< Flag to show save as popup
    char m_saveFolderBuf[256]   = {0};    ///< Buffer for save folder input
    bool m_wasInShop            = false;  ///< Flag for showing shop enter modal
    bool m_wasInPortal          = false;

    // File dialog related members
    bool        m_showFileDialog = false;  ///< Flag to show file dialog
    std::string m_selectedFolder;          ///< Selected folder path
    bool        m_showErrorPopup = false;  ///< Flag to show error popup
    std::string m_errorMessage;            ///< Error message to display

    const std::vector<std::string> m_requiredFiles = {
        "item.txt", "shop.txt", "backpack.txt", "mobloot.txt", "equipment.txt"};  ///< Required
                                                                                  ///< files for
                                                                                  ///< save files
                                                                                  ///< validation

    /**
     * @brief Validates if the selected folder contains all required files
     *
     * @param folderPath Path to the folder to check
     * @return true if all required files exist, false otherwise
     */
    bool validateFolder(const std::string& folderPath);

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