#include "core/StateManager.hpp"

#include <algorithm>
#include <iostream>

StateManager::StateManager() {}

void StateManager::PushState(std::unique_ptr<State> state)
{
    if (!m_states.empty())
    {
        m_states.back()->Pause();
    }

    m_states.push_back(std::move(state));
    m_states.back()->Init();
}

void StateManager::PopState()
{
    if (!m_states.empty())
    {
        m_states.back()->Exit();
        m_states.pop_back();

        if (!m_states.empty())
        {
            m_states.back()->Resume();
        }
    }
}

void StateManager::ChangeState(std::unique_ptr<State> state)
{
    if (!m_states.empty())
    {
        // Call Exit on the state being replaced
        m_states.back()->Exit();
        m_states.pop_back();
    }

    // Push and initialize the new state
    m_states.push_back(std::move(state));
    m_states.back()->Init();
}

void StateManager::ProcessStateChanges()
{
    for (auto& change : m_pendingChanges)
    {
        ApplyStateChange(change);
    }

    m_pendingChanges.clear();
}

void StateManager::ApplyStateChange(State::StateChange& change)
{
    switch (change.GetAction())
    {
        case State::StateAction::PUSH:
            if (change.GetNextState())
            {
                PushState(change.TakeNextState());
            }
            break;

        case State::StateAction::POP:
            PopState();
            break;

        case State::StateAction::REPLACE:
            if (change.GetNextState())
            {
                ChangeState(change.TakeNextState());
            }
            break;

        case State::StateAction::NONE:
            // No action needed
            break;
    }
}

void StateManager::ProcessEvent(const sf::Event& event)
{
    if (m_states.empty())
        return;

    // Create a collection of states to process events
    std::vector<State*> activeStates = GetActiveStateStack();

    // Process input from top to bottom until a state handles it
    for (auto it = activeStates.begin(); it != activeStates.end(); ++it)
    {
        State* state = *it;

        // Get state change request from event handling
        State::StateChange change = state->ProcessEvent(event);

        // If there's a requested state change, queue it
        if (change.GetAction() != State::StateAction::NONE)
        {
            m_pendingChanges.push_back(std::move(change));
            break;  // Stop processing input
        }

        // If state doesn't allow input propagation, stop here
        if (!state->AllowsInputPassthrough())
        {
            break;
        }
    }
}

void StateManager::Update(const sf::Time& dt)
{
    if (m_states.empty())
        return;

    // Get active state stack
    std::vector<State*> activeStates = GetActiveStateStack();

    // Update states from top to bottom
    for (auto it = activeStates.begin(); it != activeStates.end(); ++it)
    {
        State* state = *it;

        // Update the state
        State::StateChange change = state->Update(dt);

        // Queue any requested state changes
        if (change.GetAction() != State::StateAction::NONE)
        {
            m_pendingChanges.push_back(std::move(change));
        }

        // If state doesn't allow updates to pass through, stop here
        if (!state->AllowsUpdatePassthrough())
        {
            break;
        }
    }

    // Process any pending state changes
    ProcessStateChanges();
}

void StateManager::Draw(sf::RenderWindow& window)
{
    if (m_states.empty())
        return;

    // Create a vector of states to render
    std::vector<State*> visibleStates;

    // Collect states that should be rendered
    // Start from the top of the stack
    std::vector<State*> stateStack = GetStateStack();

    for (auto it = stateStack.begin(); it != stateStack.end(); ++it)
    {
        State* state = *it;
        visibleStates.push_back(state);

        // If state is not transparent, don't render states beneath it
        if (!state->IsTransparent())
        {
            break;
        }
    }

    // Draw states from bottom to top
    for (auto it = visibleStates.rbegin(); it != visibleStates.rend(); ++it)
    {
        (*it)->Draw(window);
    }
}

void StateManager::RenderUI()
{
    if (m_states.empty())
        return;

    // Create a vector of states that should render UI
    std::vector<State*> uiStates;

    // Get states with visible UI
    std::vector<State*> stateStack = GetStateStack();

    for (auto it = stateStack.begin(); it != stateStack.end(); ++it)
    {
        State* state = *it;
        uiStates.push_back(state);

        // If state doesn't have transparent UI, don't render UI beneath it
        if (!state->HasTransparentUI())
        {
            break;
        }
    }

    // Render UI from bottom to top
    for (auto it = uiStates.rbegin(); it != uiStates.rend(); ++it)
    {
        (*it)->RenderUI();
    }
}

std::vector<State*> StateManager::GetStateStack() const
{
    std::vector<State*> stateStack;

    // Just iterate through the actual m_states and extract raw pointers
    for (auto it = m_states.begin(); it != m_states.end(); ++it)
    {
        stateStack.push_back(it->get());
    }

    // Return the stack in reverse order (top state first)
    std::reverse(stateStack.begin(), stateStack.end());

    return stateStack;
}

std::vector<State*> StateManager::GetActiveStateStack() const
{
    std::vector<State*> activeStates;
    std::vector<State*> allStates = GetStateStack();

    // Keep only active states
    for (State* state : allStates)
    {
        activeStates.push_back(state);

        // Stop if state is active but doesn't allow passthrough
        if (!state->AllowsPassthrough())
        {
            break;
        }
    }

    return activeStates;
}

bool StateManager::IsEmpty() const
{
    return m_states.empty();
}

size_t StateManager::GetStateCount() const
{
    return m_states.size();
}

#ifdef DEBUG_MODE
void StateManager::PrintStateStack() const
{
    std::cout << "State Stack (" << m_states.size() << " states):" << std::endl;

    std::vector<State*> stateStack = GetStateStack();

    for (size_t i = 0; i < stateStack.size(); ++i)
    {
        std::cout << i << ": " << stateStack[i]->GetName() << std::endl;
    }
}
#endif