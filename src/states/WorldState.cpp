#include "states/WorldState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "states/ChooseCharacterState.hpp"
#include "units/AnimatedUnit.hpp"  // Include AnimatedUnit header

// Constructor
WorldState::WorldState(GameContext& context)
    : State(context),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Main Menu State");

    // TODO: setup portal spawn

    std::unique_ptr<Portal> portal =
        std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_e"),
                                 sf::Vector2f(265.0f * 0.7f, 305.0f * 0.7f),
                                 sf::Vector2f(1.0f, 1.0f));

    m_portals.emplace_back(std::move(portal));
    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_d"),
                                      sf::Vector2f(265.0f * 0.7f, 825.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f));

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_c"),
                                      sf::Vector2f(1412.0f * 0.7f, 790.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f));

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_s"),
                                      sf::Vector2f(1450.0f * 0.7f, 310.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f));

    m_portals.emplace_back(std::move(portal));
}

void WorldState::Init()
{
    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    unsigned int characterId = GetContext().GetCharacterId();
    AnimatedUnit* character = GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(characterId);

    if (character)
    {
        character->SetScale({4.0f, 4.0f});
        character->SetPosition({windowSize.x / 2.0f, windowSize.y / 2.0f + 100.0f});
        character->SetControlledByPlayer(true);
    }
    else
    {
        std::cerr << "Character not found!" << std::endl;
        return;
    }
}

State::StateChange WorldState::ProcessEvent(const sf::Event& event)
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

State::StateChange WorldState::Update(const sf::Time& dt)
{
    // first do your usual update
    for (auto& portal : m_portals)
        portal->Update(dt);

    // see if we're currently inside any portal
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    bool isInAnyPortal = false;
    if (character)
    {
        auto pos = character->GetPosition();
        for (auto& portal : m_portals)
            if (portal->isInBounds(pos))
            {
                m_currentPortalId = portal->GetId();
                isInAnyPortal     = true;
                break;
            }
    }

    // only on the frame we enter:
    if (isInAnyPortal && !m_wasInPortal)
    {
        m_showPortalEnterModal = true;
        if (character)
            character->SetControlledByPlayer(false);  // ← lock movement
    }

    m_wasInPortal = isInAnyPortal;

    // handle any pending state-change (from your buttons, etc)
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
        return std::move(m_pendingStateChange);

    return StateChange {StateAction::NONE};
}

void WorldState::Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(196, 196, 196));

    window.draw(m_backgroundSprite);

    for (auto& portal : m_portals)
    {
        portal->Draw(window);
    }
}

void WorldState::RenderUI()
{
    if (m_showPortalEnterModal)
        ImGui::OpenPopup("Do you ready to lose?");

    ImGui::SetNextWindowPos(
        ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    if (ImGui::BeginPopupModal("Do you ready to lose?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Spacing();
        if (ImGui::Button("Yes", ImVec2(100, 0)))
        {
            if (character)
                character->SetControlledByPlayer(true);
            m_showPortalEnterModal = false;
            m_currentPortalId      = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            if (character)
                character->SetControlledByPlayer(true);
            m_showPortalEnterModal = false;
            m_currentPortalId      = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void WorldState::Pause() {}

void WorldState::Resume() {}

void WorldState::Exit() {}