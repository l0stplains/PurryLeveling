#pragma once

#include "states/ChooseCharacterState.hpp"
#include "states/State.hpp"
#include "states/WorldState.hpp"

/**
 * @class MainMenuState
 * @brief State for the main menu of the game
 *
 * This state handles the main menu UI, including buttons for starting a new game,
 * loading a game, and exiting the application. It also manages background music
 * and sound effects for button interactions.
 *
 * @see State
 * @see GameContext
 * @see Button
 * @see ImGuiFileDialog
 */
class MainMenuState : public State
{
public:
    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit MainMenuState(GameContext& context);

    /**
     * @brief Initialize the main menu state
     *
     * Initializes the main menu state, including loading resources and setting up the UI.
     */
    void Init() override;

    /**
     * @brief Handle input events
     *
     * @param event The SFML event to handle
     * @param window The SFML window
     * @return State::StateChange The state change information
     */
    State::StateChange ProcessEvent(const sf::Event& event) override;

    /**
     * @brief Update the main menu state
     *
     * @param dt The time elapsed since the last update
     * @return State::StateChange The state change information
     */
    State::StateChange Update(const sf::Time& dt) override;

    /**
     * @brief Draw the main menu state
     *
     * @param window The SFML window to draw to
     */
    void Draw(sf::RenderWindow& window) override;

    /**
     * @brief Render the ImGui of the main menu state
     *
     * This method renders the ImGui elements of the main menu state.
     */
    void RenderUI() override;
    void renderNewSaveModal();
    void renderLoadSaveModal();
    void renderErrorModal();
    void renderExitConfirmationModal();

    /**
     * @brief Exit the main menu state
     *
     * This method is called when the main menu state is exited.
     * It handles thread cleanup and resource unloading.
     */
    void Exit() override;  // Important for thread cleanup!
    void Pause() override;
    void Resume() override;

private:
    sf::Sound m_backsound;         ///< Background music sound
    sf::Sound m_buttonHoverSound;  ///< Button hover sound
    sf::Sound m_buttonClickSound;  ///< Button click sound
    sf::Font  m_font;              ///< Font for UI text

    sf::Texture m_backgroundTexture;  ///< Background texture
    sf::Texture m_logoTexture;        ///< Logo texture
    sf::Texture m_logoShadowTexture;  ///< Logo shadow texture
    sf::Texture m_buttonTexture;      ///< Button texture

    sf::Sprite m_backgroundSprite;  ///< Background sprite
    sf::Sprite m_logoSprite;        ///< Logo sprite
    sf::Sprite m_logoShadowSprite;  ///< Logo shadow sprite

    Button m_newGameButton;  ///< New game button
    Button m_loadButton;     ///< Load game button
    Button m_exitButton;     ///< Exit button

    State::StateChange m_pendingStateChange;  ///< Pending state change information

    // File dialog related members
    bool        m_showFileDialog = false;     ///< Flag to show file dialog
    std::string m_selectedFolder;             ///< Selected folder path
    bool        m_showErrorPopup = false;     ///< Flag to show error popup
    std::string m_errorMessage;               ///< Error message to display
    bool        m_showExitPopup     = false;  ///< Flag to show exit confirmation popup
    bool        m_showSaveNamePopup = false;
    char        m_saveNameBuf[128]  = {0};

    std::vector<std::string> m_requiredFiles;  ///< Required
                                               ///< files for
                                               ///< save files
                                               ///< validation

    /**
     * @brief Validates if the selected folder contains all required files
     *
     * @param folderPath Path to the folder to check
     */
    void validateFolder(const std::string& folderPath);

    void checkRequiredFiles(const std::string& folderPath);

    void loadPlayerConfig(const std::string& folderPath);

    void setupNavigationGrid();

    void spawnCharacter(const PlayerConfigParser& parser);

    /**
     * @brief Parses non-player configuration files. Helper method for validateFolder.
     *
     * @param folderPath Path to the folder containing the configuration files
     */
    void parseNonPlayerConfig(const std::string& folderPath);

    /**
     * @brief Shows an error message in an ImGui popup
     *
     * @param message The error message to display
     */
    void showError(const std::string& message);
};