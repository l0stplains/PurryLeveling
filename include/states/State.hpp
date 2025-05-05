#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  // brings in sf::Time, sf::milliseconds, etc.
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>  // brings in Event, Keyboard, Mouse, etc.

#include <atomic>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "core/GameContext.hpp"
#include "core/ResourceManager.hpp"

#include "ui/Button.hpp"

#include "dungeon/Dungeon.hpp"
#include "dungeon/DungeonFactory.hpp"
#include "exception/Exception.hpp"
#include "imgui.h"
#include "parser/ItemConfigParser.hpp"
#include "parser/MobLootConfigParser.hpp"
#include "parser/PlayerConfigParser.hpp"
#include "parser/QuestConfigParser.hpp"
#include "parser/ShopConfigParser.hpp"
#include "props/Pointer.hpp"
#include "props/Portal.hpp"
#include "rng/rng.hpp"
#include "save/PlayerConfigSaver.hpp"
#include "states/Enums.hpp"
#include "units/AnimatedUnit.hpp"
#include "units/NavigationGrid.hpp"
#include "units/characters/Assassin.hpp"
#include "units/characters/Berseker.hpp"
#include "units/characters/Fighter.hpp"
#include "units/characters/Mage.hpp"
#include "units/characters/Necromancer.hpp"

/**
 * @class State
 * @brief Abstract base class for game states
 *
 * This class defines the interface for game states, including methods for
 * initialization, input handling, updating, and rendering.
 */
class State
{
public:
    /**
     * @brief Enum for state actions
     */
    class StateAction
    {
    public:
        enum Action
        {
            PUSH,     // Add a new state on top of this one
            POP,      // Remove this state
            REPLACE,  // Replace this state with a new one
            NONE      // No state change
        };
    };

    /**
     * @brief Class for state change information
     */
    class StateChange
    {
    public:
        /**
         * @brief Default constructor
         */
        StateChange();

        /**
         * @brief Constructor with action and optional next state
         *
         * @param action The action to perform
         * @param nextState Optional next state to transition to
         */
        StateChange(StateAction::Action action, std::unique_ptr<State> nextState = nullptr);

        /**
         * @brief Constructor with action and next state
         *
         * @param action The action to perform
         * @param nextState The next state to transition to
         *
         * @return StateAction::Action The action to perform
         */
        StateAction::Action GetAction() const;

        /**
         * @brief Get the next state
         *
         * @return State* The next state
         */
        State* GetNextState() const;

        /**
         * @brief Take ownership of the next state
         *
         * @return std::unique_ptr<State> The next state
         */
        std::unique_ptr<State> TakeNextState();

    private:
        StateAction::Action    m_action;     ///< The action to perform
        std::unique_ptr<State> m_nextState;  ///< The next state to transition to
    };

    /**
     * @brief Constructor
     *
     * @param context The game context to share resources
     */
    explicit State(GameContext& context);

    /**
     * @brief Virtual destructor
     */
    virtual ~State() = default;

    // Core state methods that must be implemented

    /**
     * @brief Initialize the state
     *
     * This method is called when the state is pushed onto the stack.
     */
    virtual void Init() = 0;

    /**
     * @brief Handle input events
     *
     * @param event The SFML event to handle
     * @param window The SFML window
     * @return StateChange The state change information
     */
    virtual StateChange ProcessEvent(const sf::Event& event) = 0;

    /**
     * @brief Update the state
     *
     * @param dt The time elapsed since the last update
     * @return StateChange The state change information
     */
    virtual StateChange Update(const sf::Time& dt) = 0;

    /**
     * @brief Draw the state
     *
     * @param window The SFML window to draw to
     */
    virtual void Draw(sf::RenderWindow& window) = 0;

    /**
     * @brief Exit the state
     *
     * This method is called when the state is popped from the stack.
     */
    virtual void Exit() {}

    // Optional state lifecycle hooks

    /**
     * @brief Pause the state
     */
    virtual void Pause();

    /**
     * @brief Resume the state
     */
    virtual void Resume();

    // Optional UI rendering method (for ImGui controls)

    /**
     * @brief Render the UI for this state
     *
     * This method is called after the state is drawn.
     */
    virtual void RenderUI();

    // State properties with virtual getters

    virtual bool        IsTransparent() const { return m_transparent; }
    virtual bool        HasTransparentUI() const { return m_transparentUI; }
    virtual bool        AllowsInputPassthrough() const { return m_inputPassthrough; }
    virtual bool        AllowsUpdatePassthrough() const { return m_updatePassthrough; }
    virtual bool        AllowsPassthrough() const { return m_passthrough; }
    virtual std::string GetName() const { return m_name; }

    // Setters
    void SetTransparent(bool transparent) { m_transparent = transparent; }
    void SetTransparentUI(bool transparentUI) { m_transparentUI = transparentUI; }
    void SetAllowsInputPassthrough(bool passthrough) { m_inputPassthrough = passthrough; }
    void SetAllowsUpdatePassthrough(bool passthrough) { m_updatePassthrough = passthrough; }
    void SetAllowsPassthrough(bool passthrough) { m_passthrough = passthrough; }
    void SetName(const std::string& name) { m_name = name; }

protected:
    // Protected accessor for the context

    /**
     * @brief Get the game context
     *
     * @return The game context
     */
    GameContext& GetContext();

private:
    GameContext& m_context;                      ///< The game context to share resources
    bool         m_transparent       = false;    // Should states below be rendered
    bool         m_transparentUI     = false;    // Should UI below be rendered
    bool         m_inputPassthrough  = false;    // Should input go to states below
    bool         m_updatePassthrough = false;    // Should update go to states below
    bool         m_passthrough       = false;    // Shorthand for both input and update
    std::string  m_name              = "State";  // Debug name of the state
};