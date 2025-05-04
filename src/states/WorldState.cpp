#include "states/WorldState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "rng/rng.hpp"
#include "skill/characterSkill/Mastery1/Fury.hpp"
#include "states/ChooseCharacterState.hpp"
#include "states/DungeonState.hpp"
#include "states/InventoryMenuState.hpp"
#include "states/ShopState.hpp"
#include "units/AnimatedUnit.hpp"          // Include AnimatedUnit header
#include "units/characters/Character.hpp"  // Include Character header

// Constructor
WorldState::WorldState(GameContext& context)
    : State(context),
      m_dungeonFactory(context),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("world_background")),
      m_backgroundSprite(m_backgroundTexture),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_squareButtonTexture(GetContext().GetResourceManager()->GetTexture("square_button")),
      m_exitButton(m_squareButtonTexture, {32.f, 32.f}, {0.5f, 0.5f}),
      m_inventoryButton(m_buttonTexture, {115.f, 32.f}, {0.5f, 0.5f}),
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
}

void WorldState::Init()
{
    Character* character =
        GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
    character->SetLevel(30);
    generatePortals();
    character->SetLevel(100);  // NOTES: delete this line later

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

    unsigned int  characterId = GetContext().GetCharacterId();
    AnimatedUnit* animatedCharacter =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(characterId);

    if (animatedCharacter)
    {
        animatedCharacter->SetScale({4.0f, 4.0f});
        animatedCharacter->SetPosition({windowSize.x / 2.0f + 10.f, windowSize.y / 2.0f + 69.0f});
        animatedCharacter->SetControlledByPlayer(true);
        animatedCharacter->SetActive(true);
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
                character->SetControlledByPlayer(false);
            Portal* currentPortal = nullptr;
            for (const auto& portal : m_portals)
            {
                if (portal->GetId() == m_currentPortalId)
                {
                    currentPortal = portal.get();
                }
            }
            m_pendingStateChange =
                StateChange {StateAction::PUSH,
                             std::make_unique<DungeonState>(GetContext(),
                                                            currentPortal->GetDimensionType(),
                                                            currentPortal->GetDungeon())};
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
            GetContext().SetFirstSaveState(false);
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
        ImGui::Text("Are you sure you want to exit?");
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
            std::string folder(m_saveFolderBuf);
            std::string err;
            Fury        fury;  // Dummy skill tree for now

            // grab your models out of GameContext
            auto* charPtr = GetContext().GetUnitManager()->GetUnitOfType<Character>(
                GetContext().GetCharacterId());
            PlayerConfigSaver::SaveToFolder("data/" + GetContext().GetCurrentFolderName(),
                                            *charPtr,
                                            *GetContext().GetEquipment(),
                                            *GetContext().GetBackpack(),
                                            fury,  // use dummy skill tree
                                            err);

            std::string savePath = "data/" + GetContext().GetCurrentFolderName();

            if (GetContext().GetFirstSaveState() == true)
            {
                try
                {
                    std::filesystem::create_directories(savePath);
                    GetContext().SetFirstSaveState(false);
                }
                catch (const std::exception& e)
                {
                    showError("Failed to create folder: " + std::string(e.what()));
                    return;
                }

                const std::filesystem::path cfgDir = "resources/config";
                for (auto fname : {"item.txt", "mobloot.txt", "quest.txt", "shop.txt"})
                {
                    try
                    {
                        std::filesystem::copy_file(cfgDir / fname,
                                                   std::filesystem::path(savePath) / fname,
                                                   std::filesystem::copy_options::overwrite_existing);
                    }
                    catch (const std::exception& e)
                    {
                        showError("Failed to copy " + std::string(fname) + ": " + e.what());
                        break;
                    }
                }
            }

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

void WorldState::generatePortals()
{
    m_portals.clear();
    Character* character =
        GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());

    vector<std::string> distributedRank = generateDungeonRank(character->GetLevel());

    Dungeon e = m_dungeonFactory.createDungeon(distributedRank[0], character->GetLevel(), 1000, 1000);
    Dungeon d =
        m_dungeonFactory.createDungeon(distributedRank[1], character->GetLevel(), 510000, 1000);
    Dungeon b =
        m_dungeonFactory.createDungeon(distributedRank[2], character->GetLevel(), 100000, 1000);
    Dungeon c =
        m_dungeonFactory.createDungeon(distributedRank[3], character->GetLevel(), 100000, 1000);

    std::map<string, string> portalAssets = {{"E", "portal_e"},
                                             {"D", "portal_d"},
                                             {"C", "portal_c"},
                                             {"B", "portal_b"},
                                             {"A", "portal_a"},
                                             {"S", "portal_s"},
                                             {"SPECIAL", "portal_special"}};

    // In order: Top left - bottom left - bottom right - top right
    std::unique_ptr<Portal> portal = std::make_unique<Portal>(
        GetContext().GetResourceManager()->GetTexture(portalAssets[distributedRank[0]]),
        sf::Vector2f(265.0f * 0.7f, 305.0f * 0.7f),
        sf::Vector2f(1.0f, 1.0f),
        e,
        DimensionType::CRYSTAL);

    m_portals.emplace_back(std::move(portal));
    portal = std::make_unique<Portal>(
        GetContext().GetResourceManager()->GetTexture(portalAssets[distributedRank[1]]),
        sf::Vector2f(265.0f * 0.7f, 825.0f * 0.7f),
        sf::Vector2f(1.0f, 1.0f),
        d,
        DimensionType::NORMAL);

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(
        GetContext().GetResourceManager()->GetTexture(portalAssets[distributedRank[2]]),
        sf::Vector2f(1412.0f * 0.7f, 790.0f * 0.7f),
        sf::Vector2f(1.0f, 1.0f),
        b,
        DimensionType::MISTY);

    m_portals.emplace_back(std::move(portal));

    portal = std::make_unique<Portal>(
        GetContext().GetResourceManager()->GetTexture(portalAssets[distributedRank[3]]),
        sf::Vector2f(1450.0f * 0.7f, 310.0f * 0.7f),
        sf::Vector2f(1.0f, 1.0f),
        c,
        DimensionType::LAVA);

    m_portals.emplace_back(std::move(portal));
}

vector<std::string> WorldState::generateDungeonRank(int level)
{
    // Initialize RNG
    RNG rng;
    // Define available ranks
    const vector<std::string> ranks = {"E", "D", "C", "B", "A", "S", "SPECIAL"};
    // Vector to store the result
    vector<std::string> result(4);

    // Define rank probabilities based on player level
    vector<double>      rankProbabilities;
    vector<std::string> availableRanks;

    // Set base probabilities (will include SPECIAL later)
    if (level >= 50)
    {
        rankProbabilities = {5, 5, 15, 15, 20, 40, 10};
        availableRanks    = {"E", "D", "C", "B", "A", "S", "SPECIAL"};
    }
    else if (level >= 35)
    {
        rankProbabilities = {5, 10, 20, 20, 40, 5, 10};
        availableRanks    = {"E", "D", "C", "B", "A", "S", "SPECIAL"};
    }
    else if (level >= 25)
    {
        rankProbabilities = {10, 20, 30, 40, 0, 0, 10};
        availableRanks    = {"E", "D", "C", "B", "SPECIAL"};
    }
    else if (level >= 20)
    {
        rankProbabilities = {20, 40, 40, 0, 0, 0, 10};
        availableRanks    = {"E", "D", "C", "SPECIAL"};
    }
    else if (level >= 10)
    {
        rankProbabilities = {60, 40, 0, 0, 0, 0, 10};
        availableRanks    = {"E", "D", "SPECIAL"};
    }
    else
    {
        rankProbabilities = {100, 0, 0, 0, 0, 0, 10};
        availableRanks    = {"E", "SPECIAL"};
    }

    // Position-based distribution - we want to bias lower positions toward lower ranks
    // and higher positions toward higher ranks
    for (int pos = 0; pos < 4; pos++)
    {
        // Skip processing if there's only one available rank
        if (availableRanks.size() == 1)
        {
            result[pos] = availableRanks[0];
            continue;
        }

        // Adjust probabilities based on position
        vector<double> positionAdjustedProbs = rankProbabilities;

        // Position bias factor (0.0 to 1.0)
        double positionBias = pos / 3.0;  // 0.0, 0.33, 0.67, 1.0

        // Calculate adjusted probabilities
        for (size_t i = 0; i < positionAdjustedProbs.size(); i++)
        {
            if (positionAdjustedProbs[i] > 0)
            {
                // Lower ranks (E, D) get boosted at lower positions
                // Higher ranks get boosted at higher positions
                double rankFactor = i / static_cast<double>(availableRanks.size() - 1);

                // Bias factor: increases probability of lower ranks at pos 0,
                // and higher ranks at pos 3
                double biasFactor = 1.0 + (rankFactor - 0.5) * 2.0 * (positionBias - 0.5);
                positionAdjustedProbs[i] *= biasFactor;
            }
        }

        // Normalize probabilities
        double sum = 0;
        for (double prob : positionAdjustedProbs)
        {
            sum += prob;
        }
        for (size_t i = 0; i < positionAdjustedProbs.size(); i++)
        {
            positionAdjustedProbs[i] = (sum > 0) ? positionAdjustedProbs[i] / sum * 100 : 0;
        }

        // SPECIAL rank has a fixed 10% chance regardless of position
        // So we need to adjust all other probabilities to account for this
        if (std::find(availableRanks.begin(), availableRanks.end(), "SPECIAL") != availableRanks.end())
        {
            for (size_t i = 0; i < positionAdjustedProbs.size() - 1; i++)
            {
                positionAdjustedProbs[i] *= 0.9;
            }
            positionAdjustedProbs[6] = 10.0;

            double total = 0.0;
            for (size_t i = 0; i < positionAdjustedProbs.size(); ++i)
            {
                total += positionAdjustedProbs[i];
            }
            if (total > 0.0)
            {
                for (size_t i = 0; i < positionAdjustedProbs.size(); ++i)
                {
                    positionAdjustedProbs[i] = positionAdjustedProbs[i] / total * 100.0;
                }
            }
        }

        vector<double> cumulativeProbs;
        double         cumulative = 0;
        for (size_t i = 0; i < positionAdjustedProbs.size(); i++)
        {
            cumulative += positionAdjustedProbs[i];
            cumulativeProbs.push_back(cumulative);
        }

        double randomPercent = rng.generatePercentage();

        std::string selectedRank;
        for (size_t i = 0; i < cumulativeProbs.size(); i++)
        {
            if (randomPercent <= cumulativeProbs[i] && positionAdjustedProbs[i] > 0)
            {
                selectedRank = ranks[i];
                break;
            }
        }

        result[pos] = selectedRank;
    }

    return result;
}

void WorldState::Pause()
{
    m_lastPosition = GetContext()
                         .GetUnitManager()
                         ->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId())
                         ->GetPosition();
}

void WorldState::Resume()
{
    generatePortals();
    sf::Vector2u  windowSize  = GetContext().GetWindow()->getSize();
    unsigned int  characterId = GetContext().GetCharacterId();
    AnimatedUnit* animatedCharacter =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(characterId);

    if (animatedCharacter)
    {
        animatedCharacter->Reset();
        animatedCharacter->SetNavGrid(*GetContext().GetNavigationGrid());
        NavigationGrid* nav     = GetContext().GetNavigationGrid();
        auto            navGrid = nav->GetGrid();
        animatedCharacter->SetActive(true);
        animatedCharacter->SetScale({4.0f, 4.0f});
        animatedCharacter->SetPosition(m_lastPosition);
        animatedCharacter->SetControlledByPlayer(true);
    }
    else
    {
        std::cerr << "Character not found!" << std::endl;
        return;
    }
}

void WorldState::Exit()
{
    GetContext().GetUnitManager()->Clear();

    // Save everything related to the user loaded thing
}