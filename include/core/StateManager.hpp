#pragma once
#include <memory>
#include <stack>
#include <vector>

#include "states/State.hpp"

// #define DEBUG_MODE

/**
 * @class StateManager
 * @brief Manages the state stack and handles state transitions.
 *
 * This class is responsible for managing the stack of game states,
 * handling state transitions, and integrating with the game loop.
 *
 * @see State
 */
class StateManager
{
public:
    /**
     * @brief Constructor
     *
     * @param context Reference to the game context
     */
    StateManager();

    // State stack operations

    /**
     * @brief Push a new state onto the stack
     *
     * @param state Unique pointer to the new state
     */
    void PushState(std::unique_ptr<State> state);

    /**
     * @brief Pop the current state from the stack
     */
    void PopState();

    /**
     * @brief Replace the current state with a new one
     *
     * @param state Unique pointer to the new state
     */
    void ChangeState(std::unique_ptr<State> state);

    /**
     * @brief Clear the state stack
     */
    void ProcessStateChanges();

    // Game loop integration

    /**
     * @brief Process input events
     *
     * @param event The SFML event to process
     * @param window The SFML window
     */
    void ProcessEvent(const sf::Event& event);

    /**
     * @brief Update the current state
     *
     * @param dt The time elapsed since the last update
     */
    void Update(const sf::Time& dt);

    /**
     * @brief Draw the current state
     *
     * @param window The SFML window to draw to
     */
    void Draw(sf::RenderWindow& window);

    /**
     * @brief Render the UI of the current state
     */
    void RenderUI();

    // State stack access (non-owning pointers)

    /**
     * @brief Get the current state
     *
     * @return Pointer to the current state
     */
    std::vector<State*> GetStateStack() const;

    /**
     * @brief Get the active state stack
     *
     * @return Vector of pointers to active states
     */
    std::vector<State*> GetActiveStateStack() const;

    // Utility methods

    /**
     * @brief Check if the state stack is empty
     *
     * @return true if the stack is empty, false otherwise
     */
    bool IsEmpty() const;

    /**
     * @brief Get the number of states in the stack
     *
     * @return The number of states in the stack
     */
    size_t GetStateCount() const;

#ifdef DEBUG_MODE
    /**
     * @brief Print the current state stack for debugging
     */
    void PrintStateStack() const;
#endif

private:
    std::vector<std::unique_ptr<State>> m_states;  ///< Vector of unique pointers to states (stack
                                                   ///< as a vector)
    std::vector<State::StateChange> m_pendingChanges;  ///< Vector of pending state changes

    /**
     * @brief Apply a state change to the state stack
     *
     * @param change The state change to apply
     */
    void ApplyStateChange(State::StateChange& change);
};