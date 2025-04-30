#include "states/InventoryMenuState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"

// Constructor
InventoryMenuState::InventoryMenuState(GameContext& context)
    : State(context),
      m_inventoryMenu(context),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_squareButtonTexture(GetContext().GetResourceManager()->GetTexture("square_button")),
      m_exitButton(m_squareButtonTexture, {32.f, 32.f}, {0.5f, 0.5f}),
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("inventory_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Inventory Menu State");
}

void InventoryMenuState::Init()
{
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

    m_exitButton.setText("X", m_font, 24);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    // Set button callbacks
    m_exitButton.setOnClickCallback([this]() {
        GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(true);
        m_pendingStateChange = StateChange {StateAction::POP};
    });

    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});
}

State::StateChange InventoryMenuState::ProcessEvent(const sf::Event& event)
{
    // Reset pending state change if it was handled
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        StateChange change   = std::move(m_pendingStateChange);
        m_pendingStateChange = {StateAction::NONE};
        return change;
    }

    return StateChange {};
}

State::StateChange InventoryMenuState::Update(const sf::Time& dt)
{
    sf::RenderWindow* window = GetContext().GetWindow();

    /*
    if (!m_showModal)
    {
        m_exitButton.update(*window);
    }
    */

    m_exitButton.update(*window);
    // Check if we have a pending state change from a button callback
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        return std::move(m_pendingStateChange);
    }

    return StateChange {StateAction::NONE};
}

void InventoryMenuState::Draw(sf::RenderWindow& window)
{
    window.clear({20, 20, 20});

    window.draw(m_backgroundSprite);

    m_exitButton.draw(window);

    m_inventoryMenu.Render();
}

void InventoryMenuState::RenderUI() {}

void InventoryMenuState::Exit() {}