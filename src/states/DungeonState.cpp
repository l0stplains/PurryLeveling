#include "states/DungeonState.hpp"

#include <cmath>
#include <iostream>

#include "units/AnimatedUnit.hpp"

// a little helper
float calculateDistance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    sf::Vector2f diff = a - b;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    // maybe i can use this instead: return std::hypot(diff.x, diff.y);
}

// Constructor
DungeonState::DungeonState(GameContext& context, DimensionType dimension, std::string rank)
    : State(context),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_attackButton(m_buttonTexture, {32.f, 500}, {1.f, 1.f}),
      m_useItemButton(m_buttonTexture, {32.f, 580.f}, {1.f, 1.f}),
      m_exitButton(m_buttonTexture, {32.f, 660.f}, {1.f, 1.f}),
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_boldFont(GetContext().GetResourceManager()->GetFont("main_bold_font")),
      m_chamberExitArea(GetContext().GetResourceManager()->GetTexture("empty_prop")),
      m_character(dynamic_cast<AnimatedUnit*>(
          GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId()))),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Dungeon State");

    ResourceManager* resources = GetContext().GetResourceManager();
    switch (dimension)
    {
        case DimensionType::LAVA:
            m_buttonTexture     = resources->GetTexture("lava_button");
            m_backgroundTexture = resources->GetTexture("lava_base");
            break;
        case DimensionType::MISTY:
            m_buttonTexture     = resources->GetTexture("misty_button");
            m_backgroundTexture = resources->GetTexture("misty_base");
            break;
        case DimensionType::CRYSTAL:
            m_buttonTexture     = resources->GetTexture("crystal_button");
            m_backgroundTexture = resources->GetTexture("crystal_base");
            break;
        case DimensionType::NORMAL:
            m_buttonTexture     = resources->GetTexture("normal_button");
            m_backgroundTexture = resources->GetTexture("normal_base");
            break;
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_attackButton  = Button(m_buttonTexture, {32.f, 500}, {1.f, 1.f}),
    m_useItemButton = Button(m_buttonTexture, {32.f, 580.f}, {1.f, 1.f}),
    m_exitButton    = Button(m_buttonTexture, {32.f, 660.f}, {1.f, 1.f}),

    // setup door enter area
        m_chamberExitArea.setOrigin({0, 0});
    m_chamberExitArea.setPosition({628.f, 350.f});
}

void DungeonState::Init()
{
    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    m_attackButton.setText("Attack", m_font, 24);
    m_attackButton.setHoverSound(m_buttonHoverSound);
    m_attackButton.setClickSound(m_buttonClickSound);

    m_useItemButton.setText("Use Item", m_font, 24);
    m_useItemButton.setHoverSound(m_buttonHoverSound);
    m_useItemButton.setClickSound(m_buttonClickSound);

    m_exitButton.setText("Surrender", m_font, 24);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    m_exitButton.setOnClickCallback([this]() { m_showExitPopup = true; });
    m_attackButton.setOnClickCallback([this]() {
        Unit* closestMob;
        float distance = 0;
        if (m_mobs.size())
        {
            distance   = calculateDistance(m_mobs[0]->GetPosition(), m_character->GetPosition());
            closestMob = m_mobs[0];
        }
        for (auto mob : m_mobs)
        {
            float tempDistance = calculateDistance(mob->GetPosition(), m_character->GetPosition());
            if (tempDistance < distance)
            {
                distance   = tempDistance;
                closestMob = mob;
            }
        }

        m_character->Attack(*closestMob);
    });

    if (m_character)
    {
        m_character->SetScale({8.0f, 8.0f});
        m_character->SetPosition({windowSize.x / 2.0f - 400.f * windowSize.x / 1820,
                                  windowSize.y / 2.0f + 780.0f * windowSize.y / 1024});
        m_character->SetControlledByPlayer(false);
    }
    else
    {
        std::cerr << "Character not found!" << std::endl;
        return;
    }
}

State::StateChange DungeonState::ProcessEvent(const sf::Event& event)
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

State::StateChange DungeonState::Update(const sf::Time& dt)
{
    // see if we're currently inside any portal
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    bool isInChamberExit = false;
    if (character)
    {
        auto          pos           = character->GetPosition();
        sf::FloatRect chamberBounds = m_chamberExitArea.getGlobalBounds();
        if (chamberBounds.contains(pos))
        {
            isInChamberExit = true;
        }
    }

    if (isInChamberExit && !m_wasInChamberExit)
    {
        if (character)
            character->SetControlledByPlayer(false);  // ← lock movement
    }

    m_wasInChamberExit = isInChamberExit;

    if (!(m_showExitPopup))
    {
        m_exitButton.update(*GetContext().GetWindow());
        m_attackButton.update(*GetContext().GetWindow());
        m_useItemButton.update(*GetContext().GetWindow());
    }

    // handle any pending state-change (from your buttons, etc)
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
        return std::move(m_pendingStateChange);

    return StateChange {StateAction::NONE};
}

void DungeonState::Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(196, 196, 196));

    window.draw(m_backgroundSprite);

    window.draw(m_chamberExitArea);
    m_exitButton.draw(window);
    m_attackButton.draw(window);
    m_useItemButton.draw(window);
}

void DungeonState::RenderUI()
{
    // Display exit confirmation popup
    if (m_showExitPopup)
    {
        ImGui::OpenPopup("Surrender Confirmation");
        m_showExitPopup = false;  // prevent reopening every frame
    }
    if (ImGui::BeginPopupModal("Surrender Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Do you really want to surrender?");
        ImGui::Dummy(ImVec2(0, 4.0f));  // Add some space
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 4.0f));  // Add some space
        float buttonWidth = 60.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - 2 * buttonWidth - 10.0f);
        ImGui::SetCursorPosX(buttonPosX);
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red color
        // ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));  //
        // Lighter red ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
        // // Darker red

        if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
            m_pendingStateChange = StateChange {StateAction::POP};
        }
        // ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::SetCursorPosX(buttonPosX + buttonWidth + 10.0f);

        if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void DungeonState::Pause() {}

void DungeonState::Resume() {}

void DungeonState::Exit()
{
    GetContext().GetUnitManager()->Clear();

    // Save everything related to the user loaded thing
}