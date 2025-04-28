#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "parser/ResourceConfigParser.hpp"
#include "states/State.hpp"

/**
 * @class LoadingState
 * @brief State for loading game resources
 *
 * This state handles the loading of game resources in a separate thread
 * and provides a loading screen with progress feedback.
 *
 * @see State
 * @see ResourceConfigParser
 * @see GameContext
 */
class LoadingState : public State
{
public:
    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit LoadingState(GameContext& context);

    /**
     * @brief Initialize the loading state
     *
     * Initializes the loading state, including loading resources and setting up the UI.
     */
    void Init() override;

    /**
     * @brief Process events
     *
     * @param event The SFML event to handle
     * @return StateChange The state change information
     */
    StateChange ProcessEvent(const sf::Event& event) override;

    /**
     * @brief Update the loading state
     *
     * @param dt The time elapsed since the last update
     * @return StateChange The state change information
     */
    StateChange Update(const sf::Time& dt) override;

    /**
     * @brief Draw the loading state
     *
     * @param window The SFML window to draw to
     */
    void Draw(sf::RenderWindow& window) override;

    /**
     * @brief Render the UI of the loading state
     *
     * This method renders the UI elements of the loading state.
     */
    void RenderUI() override;

    /**
     * @brief Exit the loading state
     *
     * This method is called when the loading state is exited.
     * It handles thread cleanup and resource unloading.
     */
    void Exit() override;

private:
    /**
     * @brief Load resources in a separate thread
     *
     * This method is responsible for loading resources in a separate thread.
     * It updates the loading progress and handles errors.
     */
    void LoadResources();

    /**
     * @brief Load resources from the configuration file
     *
     * This method loads resources from the specified configuration file.
     * It handles errors and updates the loading progress.
     *
     * @param configPath The path to the configuration file
     * @return true if loading was successful, false otherwise
     */
    bool LoadResourcesFromConfig(const std::string& configPath);

    /**
     * @brief Resource configuration parser
     *
     * This parser is responsible for parsing the resource configuration file.
     */
    std::unique_ptr<ResourceConfigParser> m_resourceParser;

    // Loading thread resources

    std::thread m_loadingThread;              ///< Thread for loading resources
    std::mutex  m_progressMutex;              ///< Mutex for thread-safe progress updates
    int         m_totalResources  = 0;        ///< Total number of resources to load
    int         m_resourcesLoaded = 0;        ///< Number of resources loaded so far
    float       m_loadingProgress;            ///< Loading progress (0.0 to 1.0)
    std::string m_currentLoadingDescription;  ///< Description of the current loading step
    bool        m_loadingComplete;            ///< Flag indicating if loading is complete
    bool m_loadingTransitionStarted = false;  ///< Flag indicating if the loading transition has
                                              ///< started
    bool m_loadingError     = false;  ///< Flag indicating if there was an error during loading
    bool m_showErrorDialog  = false;  ///< Flag indicating if the error dialog should be shown
    bool m_closeApplication = false;  ///< Flag indicating if the application should be closed
    std::string m_errorMessage;       ///< Error message to display in the dialog
    sf::Clock   m_loadingTimer;       ///< Timer for loading duration

    // Visual elements

    sf::Texture& m_logoTexture;  ///< Texture for the logo
    sf::Sprite   m_logoSprite;   ///< Sprite for the logo
};