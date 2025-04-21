// GameContext.h
#pragma once
#include <SFML/Graphics.hpp>

#include "core/EntityManager.hpp"
#include "core/ResourceManager.hpp"

class StateManager;  // forward declaration
// class EntityManager;  // forward declaration

/**
 * @class GameContext
 * @brief Context class for sharing resources between states
 *
 * This class provides a way to share resources such as the resource manager,
 * entity manager, and window between different game states. It also tracks of all necessary
 * information.
 *
 */
class GameContext
{
public:
    /**
     * @brief Default constructor for GameContext
     */
    GameContext();

    /**
     * @brief Destructor for GameContext
     */
    ~GameContext();

    // Getters

    /**
     * @brief Get the resource manager
     *
     * @return ResourceManager* Pointer to the resource manager
     */
    ResourceManager* GetResourceManager() const;

    /**
     * @brief Get the entity manager
     *
     * @return EntityManager* Pointer to the entity manager
     */
    EntityManager* GetEntityManager() const;

    /**
     * @brief Get the state manager
     *
     * @return StateManager* Pointer to the state manager
     */
    StateManager* GetStateManager() const;

    /**
     * @brief Get the window
     *
     * @return sf::RenderWindow* Pointer to the SFML render window
     */
    sf::RenderWindow* GetWindow() const;

    /**
     * @brief Get the current frames per second (FPS)
     *
     * @return float Current FPS
     */
    float GetCurrentFPS() const;

    /**
     * @brief Get the frame counter
     *
     * @return unsigned int Current frame counter
     */
    unsigned int GetFrameCounter() const;

    /**
     * @brief Get the FPS timer
     *
     * @return sf::Time Current FPS timer
     */
    sf::Time GetFPSTimer() const;

    // Setters

    /**
     * @brief Set the resource manager
     *
     * @param resourceManager Pointer to the resource manager
     */
    void SetResourceManager(ResourceManager* resourceManager);

    /**
     * @brief Set the entity manager
     *
     * @param entityManager Pointer to the entity manager
     */
    void SetEntityManager(EntityManager* entityManager);

    /**
     * @brief Set the state manager
     *
     * @param stateManager Pointer to the state manager
     */
    void SetStateManager(StateManager* stateManager);

    /**
     * @brief Set the window
     *
     * @param window Pointer to the SFML render window
     */
    void SetWindow(sf::RenderWindow* window);

    /**
     * @brief Set the vertical sync (VSync) enabled state
     *
     * @param enabled true to enable VSync, false to disable
     */
    void SetVSyncEnabled(bool enabled);

    // FPS tracking

    /**
     * @brief Update the FPS tracking
     *
     * @param dt Time elapsed since the last frame
     */
    void UpdateFPS(const sf::Time& dt);

    /**
     * @brief Toggle VSync on or off
     */
    void ToggleVSync();

private:
    ResourceManager* m_resourceManager;  ///< Pointer to the resource manager
    EntityManager*   m_entityManager;    ///< Pointer to the entity manager
    StateManager*    m_stateManager;     ///< Pointer to the state manager

    sf::RenderWindow* m_window;  ///< Pointer to the SFML render window

    // FPS tracking
    unsigned int m_frameCounter = 0;  ///< Frame counter
    sf::Time     m_fpsTimer;          ///< Timer for FPS calculation
    float        m_currentFPS = 0.f;  ///< Current FPS

    bool m_vsyncEnabled = false;  ///< Flag for VSync enabled state
};