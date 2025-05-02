#pragma once
#include <SFML/Graphics.hpp>

#include "core/ImGuiManager.hpp"
#include "core/ResourceManager.hpp"
#include "core/StateManager.hpp"
#include "core/UnitManager.hpp"

#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "items/ItemManager.hpp"
#include "parser/MobLootConfigParser.hpp"

/**
 * @class Game
 * @brief Main game class that initializes and runs the game loop.
 *
 * This class handles the game window, timing, and manages the game states.
 * It initializes the game context and provides methods for processing events,
 * updating game logic, and rendering graphics.
 */
class Game
{
public:
    /**
     * @brief Constructor for the Game class.
     *
     * Initializes the game window, managers, and context.
     */
    Game();

    /**
     * @brief Destructor for the Game class.
     *
     * Cleans shuts down the gui.
     */
    ~Game();

    void Run();

private:
    // Window settings
    sf::RenderWindow m_window;        ///< The main game window
    bool             m_vsyncEnabled;  ///< Flag for vertical sync

    // Game timing
    sf::Clock m_gameClock;  ///< Clock for measuring time

    // Game systems
    GameContext     m_context;          ///< Context for sharing resources between states
    ResourceManager m_resourceManager;  ///< Resource manager for loading and managing game assets
    UnitManager     m_unitManager;      ///< Unit manager for managing game units
    StateManager    m_stateManager;     ///< State manager for handling game states
    ItemManager     m_itemManager;
    MobLootConfigParser m_mobLootConfigParser;
    Backpack            m_backpack;
    Equipment           m_equipment;
    ImGuiManager        m_gui;  ///< ImGui backend manager for rendering UI

    /**
     * @brief Processes events from the window and updates the game state.
     */
    void ProcessEvents();

    /**
     * @brief Updates the game state based on the elapsed time.
     *
     * @param deltaTime The time elapsed since the last update.
     */
    void Update(const sf::Time& deltaTime);

    /**
     * @brief Renders the game graphics to the window.
     */
    void Render();
};