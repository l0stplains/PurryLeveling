#include "states/State.hpp"

// StateChange implementation
State::StateChange::StateChange() : m_action(StateAction::NONE), m_nextState(nullptr) {}

State::StateChange::StateChange(StateAction::Action action, std::unique_ptr<State> nextState)
    : m_action(action), m_nextState(std::move(nextState))
{}

State::StateAction::Action State::StateChange::GetAction() const
{
    return m_action;
}

State* State::StateChange::GetNextState() const
{
    return m_nextState.get();
}

std::unique_ptr<State> State::StateChange::TakeNextState()
{
    return std::move(m_nextState);
}

// State implementation
State::State(GameContext& context) : m_context(context) {}

void State::Pause()
{
    // Default implementation does nothing
}

void State::Resume()
{
    // Default implementation does nothing
}

void State::RenderUI()
{
    // Default implementation does nothing
}

GameContext& State::GetContext()
{
    return m_context;
}