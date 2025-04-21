#pragma once
#include <memory>
#include <stack>
#include <vector>

#include "core/State.hpp"

/**
 * Manages game states with proper OOP design
 */
class StateManager
{
public:
    explicit StateManager(const GameContext& context);

    // State stack operations
    void PushState(std::unique_ptr<State> state);
    void PopState();
    void ChangeState(std::unique_ptr<State> state);
    void ProcessStateChanges();

    // Game loop integration
    void ProcessEvent(const sf::Event& event, const sf::Window& window);
    void Update(const sf::Time& dt);
    void Draw(sf::RenderWindow& window);
    void RenderUI();

    // State stack access (non-owning pointers)
    std::vector<State*> GetStateStack() const;
    std::vector<State*> GetActiveStateStack() const;

    // Utility methods
    bool   IsEmpty() const;
    size_t GetStateCount() const;

#ifdef DEBUG_MODE
    void PrintStateStack() const;
#endif

private:
    std::vector<std::unique_ptr<State>> m_states;
    std::vector<State::StateChange>     m_pendingChanges;
    const GameContext&                  m_context;

    void ApplyStateChange(State::StateChange& change);
};