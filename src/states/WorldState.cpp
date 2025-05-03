#include "states/WorldState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "states/ChooseCharacterState.hpp"
#include "states/InventoryMenuState.hpp"
#include "states/ShopState.hpp"
#include "units/AnimatedUnit.hpp"  // Include AnimatedUnit header

// Constructor
WorldState::WorldState(GameContext& context)
    : State(context),
      m_dungeonFactory(*context.GetItemManager(), *context.GetMobLootConfigParser()),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_squareButtonTexture(GetContext().GetResourceManager()->GetTexture("square_button")),
      m_exitButton(m_squareButtonTexture, {32.f, 32.f}, {0.5f, 0.5f}),
      m_inventoryButton(m_buttonTexture, {112.f, 32.f}, {0.5f, 0.5f}),
      m_skillTreeButton(m_buttonTexture, {236.f, 32.f}, {0.5f, 0.5f}),
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_boldFont(GetContext().GetResourceManager()->GetFont("main_bold_font")),
      m_doorEnterArea(GetContext().GetResourceManager()->GetTexture("empty_prop")),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("World Menu State");

    // setup door enter area
    m_doorEnterArea.setOrigin({0, 0});
    m_doorEnterArea.setPosition({628.f, 350.f});

    // TODO: setup portal spawn

    Dungeon e = m_dungeonFactory.createDungeon("E", 10, 1000, 1000);
    Dungeon d = m_dungeonFactory.createDungeon("D", 20, 510000, 1000);
    Dungeon b = m_dungeonFactory.createDungeon("B", 30, 100000, 1000);
    Dungeon c = m_dungeonFactory.createDungeon("C", 40, 100000, 1000);

    std::unique_ptr<Portal> portal =
        std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_e"),
                                 sf::Vector2f(265.0f * 0.7f, 305.0f * 0.7f),
                                 sf::Vector2f(1.0f, 1.0f),
                                 e);

    m_portals.emplace_back(std::move(portal));
    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_d"),
                                      sf::Vector2f(265.0f * 0.7f, 825.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f),
                                      d);

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_c"),
                                      sf::Vector2f(1412.0f * 0.7f, 790.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f),
                                      b);

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(GetContext().GetResourceManager()->GetTexture("portal_s"),
                                      sf::Vector2f(1450.0f * 0.7f, 310.0f * 0.7f),
                                      sf::Vector2f(1.0f, 1.0f),
                                      c);

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

    m_exitButton.setText("X", m_font, 24);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    m_inventoryButton.setText("Inventory", m_font, 24);
    m_inventoryButton.setHoverSound(m_buttonHoverSound);
    m_inventoryButton.setClickSound(m_buttonClickSound);

    m_skillTreeButton.setText("Skill Tree", m_font, 24);
    m_skillTreeButton.setHoverSound(m_buttonHoverSound);
    m_skillTreeButton.setClickSound(m_buttonClickSound);
    // Set button callbacks
    m_exitButton.setOnClickCallback([this]() { m_showExitPopup = true; });
    m_inventoryButton.setOnClickCallback([this]() {
        GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(false);
        m_pendingStateChange =
            StateChange {StateAction::PUSH, std::make_unique<InventoryMenuState>(GetContext())};
    });

    unsigned int characterId = GetContext().GetCharacterId();
    AnimatedUnit* character = GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(characterId);

    if (character)
    {
        character->SetScale({4.0f, 4.0f});
        character->SetPosition({windowSize.x / 2.0f + 10.f, windowSize.y / 2.0f + 69.0f});
        character->SetControlledByPlayer(true);
        character->SetActive(true);
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
    bool isInFrontDoor = false;
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

        sf::FloatRect doorBounds = m_doorEnterArea.getGlobalBounds();
        if (doorBounds.contains(pos))
        {
            isInFrontDoor = true;
        }
    }

    if (isInFrontDoor && !m_wasInShop)
    {
        m_showShopEnterModal = true;
        if (character)
            character->SetControlledByPlayer(false);  // ← lock movement
    }

    // only on the frame we enter:
    if (isInAnyPortal && !m_wasInPortal)
    {
        m_showPortalEnterModal = true;
        if (character)
            character->SetControlledByPlayer(false);  // ← lock movement
    }

    m_wasInPortal = isInAnyPortal;
    m_wasInShop   = isInFrontDoor;

    if (!(m_showErrorPopup || m_showExitPopup || m_showFileDialog || m_showPortalEnterModal ||
          m_showShopEnterModal))
    {
        m_exitButton.update(*GetContext().GetWindow());
        m_inventoryButton.update(*GetContext().GetWindow());
        m_skillTreeButton.update(*GetContext().GetWindow());
    }

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
    window.draw(m_doorEnterArea);
    m_exitButton.draw(window);
    m_inventoryButton.draw(window);
    m_skillTreeButton.draw(window);
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

    if (m_showShopEnterModal)
        ImGui::OpenPopup("Do you want to enter the shop?");

    ImGui::SetNextWindowPos(
        ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(
            "Do you want to enter the shop?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Spacing();
        if (ImGui::Button("Yes", ImVec2(100, 0)))
        {
            if (character)
                character->SetControlledByPlayer(true);
            m_showShopEnterModal = false;
            ImGui::CloseCurrentPopup();
            GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(false);
            m_pendingStateChange =
                StateChange {StateAction::PUSH, std::make_unique<ShopState>(GetContext())};
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            if (character)
                character->SetControlledByPlayer(true);
            m_showShopEnterModal = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    // Process file dialog if it's active
    if (m_showFileDialog)
    {
        ImVec2 screenSize =
            ImVec2(GetContext().GetWindow()->getSize().x, GetContext().GetWindow()->getSize().y);
        ImVec2 dialogSize = ImVec2(800, 600);
        ImVec2 dialogPos =
            ImVec2((screenSize.x - dialogSize.x) * 0.5f, (screenSize.y - dialogSize.y) * 0.5f);

        // Set next window position and size
        ImGui::SetNextWindowPos(dialogPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(dialogSize, ImGuiCond_Always);

        // Display the file dialog
        if (ImGuiFileDialog::Instance()->Display("ChooseFolderDlgKey"))
        {
            // When user clicks OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                m_selectedFolder = ImGuiFileDialog::Instance()->GetCurrentPath();

                // Validate the folder
                if (validateFolder(m_selectedFolder))
                {
                    std::cout << "Selected valid save folder: " << m_selectedFolder << std::endl;
                }
                else
                {
                    // Invalid folder - show error
                    showError("Selected folder is missing required save files!");
                }
            }

            // Close dialog
            m_showFileDialog = false;
            ImGuiFileDialog::Instance()->Close();
        }
    }

    // Display error popup if needed
    // Open popup once
    if (m_showErrorPopup)
    {
        ImGui::OpenPopup("Error");
        m_showErrorPopup = false;  // prevent reopening every frame
    }

    if (ImGui::BeginPopupModal("Error",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("%s", m_errorMessage.c_str());
        ImGui::Separator();
        float buttonWidth = 120.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f;
        ImGui::SetCursorPosX(buttonPosX);

        if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Display exit confirmation popup
    if (m_showExitPopup)
    {
        ImGui::OpenPopup("Exit Confirmation");
        m_showExitPopup = false;  // prevent reopening every frame
    }
    if (ImGui::BeginPopupModal("Exit Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Do you want to save before you exit?");
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

bool WorldState::validateFolder(const std::string& folderPath)
{
    // Check if all required files exist in the folder
    for (const auto& requiredFile : m_requiredFiles)
    {
        std::filesystem::path filePath = std::filesystem::path(folderPath) / requiredFile;

        if (!std::filesystem::exists(filePath))
        {
            std::cout << "Required file already exist: " << requiredFile << std::endl;
            return false;
        }
    }

    // TODO: Save GameContext to the files

    return true;
}

void WorldState::showError(const std::string& message)
{
    m_showErrorPopup = true;
    m_errorMessage   = message;
}

void WorldState::Pause() {}

void WorldState::Resume() {}

void WorldState::Exit()
{
    GetContext().GetUnitManager()->Clear();

    // Save everything related to the user loaded thing
}