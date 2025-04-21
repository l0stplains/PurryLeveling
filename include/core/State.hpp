#pragma once
#include <SFML/Graphics.hpp>

#include <memory>

#include "core/GameContext.hpp"

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
    explicit State(const GameContext& context);

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
    virtual StateChange HandleInput(const sf::Event& event, const sf::Window& window) = 0;

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

    virtual void Pause();
    virtual void Resume();

    // Optional UI rendering method (for ImGui controls)

    /**
     * @brief Render the UI for this state
     *
     * This method is called after the state is drawn.
     */
    virtual void RenderUI();

    // State properties with virtual getters
    virtual bool IsTransparent() const { return false; }     // Should states below be rendered
    virtual bool HasTransparentUI() const { return false; }  // Should UI below be rendered
    virtual bool AllowsInputPassthrough() const
    {
        return false;
    }  // Should input go to states below
    virtual bool AllowsUpdatePassthrough() const
    {
        return false;
    }  // Should update go to states below
    virtual bool AllowsPassthrough() const { return false; }  // Shorthand for both input and update

    // For debugging
    virtual std::string GetName() const { return "State"; }

protected:
    // Protected accessor for the context
    const GameContext& GetContext() const;

private:
    const GameContext& m_context;  ///< The game context to share resources
};