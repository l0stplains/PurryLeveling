#include "states/WorldState.hpp"

// Constructor
WorldState::WorldState(GameContext& context)
    : State(context),
      m_backsound(GetContext().GetResourceManager()->GetSoundBuffer("world_backsound")),
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
    m_backsound.setLooping(true);
    m_backsound.setVolume(25);
    m_backsound.play();

    Character* character =
        GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
    generatePortals();

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

    m_skillTreeButton.setOnClickCallback([this]() {
        // Deactivate the player's current unit if needed
        GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(false);
        // Push the SkillTreeState onto the state stack
        m_pendingStateChange =
            StateChange {StateAction::PUSH, std::make_unique<SkillTreeState>(GetContext())};
    });

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

    if (!(m_showErrorPopup || m_showExitPopup || m_showPortalEnterModal || m_showShopEnterModal))
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
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    // Render different modals as needed
    if (m_showPortalEnterModal)
        RenderPortalEnterModal();

    if (m_showShopEnterModal)
        RenderShopEnterModal(character);

    // Error popup rendering
    if (m_showErrorPopup)
        RenderErrorModal();

    // Exit confirmation popup
    if (m_showExitPopup)
        RenderExitConfirmationModal();

    // Add player info window
    RenderPlayerInfo();
}

// Helper function for pushing standard modal styles
void WorldState::PushModalStyle(const ImVec4& borderColor)
{
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
}

// Helper function for popping standard modal styles
void WorldState::PopModalStyle()
{
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(3);
}

// Helper function for creating standard buttons
bool WorldState::RenderStyledButton(const char* label, const ImVec2& size, bool isAction)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    ImVec4 buttonColor, hoverColor, activeColor, borderColor;

    if (isAction)
    {
        // Green action button
        buttonColor = ImVec4(0.0f, 0.3f, 0.0f, 1.0f);
        hoverColor  = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);
        activeColor = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
        borderColor = ImVec4(0.0f, 0.7f, 0.0f, 1.0f);
    }
    else
    {
        // Gray cancel/back button
        buttonColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
        hoverColor  = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        activeColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        borderColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    }

    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);

    bool clicked = ImGui::Button(label, size);

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    return clicked;
}

void WorldState::RenderPortalEnterModal()
{
    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(320, 270));

    // Find current portal
    Portal* currentPortal = nullptr;
    for (const auto& portal : m_portals)
    {
        if (portal->GetId() == m_currentPortalId)
        {
            currentPortal = portal.get();
            break;
        }
    }

    if (!currentPortal)
    {
        return;  // Portal not found, don't show modal
    }

    // Get portal info and set border color based on rank
    std::string dungeonRank    = currentPortal->GetDungeon().getRank();
    int         entranceFee    = currentPortal->GetDungeon().getEntranceFee();
    int         minPlayerLevel = currentPortal->GetDungeon().getMinPlayerLevel();

    // Set border color based on dungeon rank with new color scheme
    ImVec4 rankColor;
    if (dungeonRank == "S")
        rankColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);  // Red
    else if (dungeonRank == "A")
        rankColor = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);  // Orange
    else if (dungeonRank == "B")
        rankColor = ImVec4(0.6f, 0.2f, 0.8f, 1.0f);  // Purple
    else if (dungeonRank == "C")
        rankColor = ImVec4(0.0f, 0.7f, 0.8f, 1.0f);  // Cyan
    else if (dungeonRank == "D")
        rankColor = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);  // Green
    else if (dungeonRank == "E")
        rankColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // Grey
    else if (dungeonRank == "SPECIAL")
        rankColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow
    else
        rankColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);  // Default border

    // Apply modal styling with rank-specific border
    PushModalStyle(rankColor);

    ImGui::OpenPopup("Start Dungeon");
    AnimatedUnit* character =
        GetContext().GetUnitManager()->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId());

    if (ImGui::BeginPopupModal("Start Dungeon",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                                   ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar))
    {
        const float spacing        = 3.0f;
        const float sectionSpacing = 3.0f;

        // Title header with rank
        ImGui::SetWindowFontScale(1.3f);
        if (dungeonRank == "SPECIAL")
        {
            ImGui::TextColored(rankColor, "Special Dungeon");
        }
        else
        {
            ImGui::TextColored(rankColor, "Rank %s Dungeon", dungeonRank.c_str());
        }
        ImGui::SetWindowFontScale(1.1f);

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Entrance fee with gold color
        ImGui::Text("Entrance Fee:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "$%d", entranceFee);

        // Level requirement
        ImGui::Text("Required Level:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%d", minPlayerLevel);

        ImGui::SetWindowFontScale(1.0f);
        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Check requirements
        Character* playerChar =
            GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
        int playerLevel = playerChar ? playerChar->GetLevel() : 0;
        int playerGold  = playerChar ? playerChar->GetGold() : 0;

        bool levelRequirementMet = playerLevel >= minPlayerLevel;
        bool goldRequirementMet  = playerGold >= entranceFee;
        bool allRequirementsMet  = levelRequirementMet && goldRequirementMet;

        // Display message area - always shown regardless of requirements
        if (!allRequirementsMet)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));

            if (!levelRequirementMet && !goldRequirementMet)
            {
                ImGui::TextWrapped("You need to be level %d and have %d gold to enter this "
                                   "dungeon.",
                                   minPlayerLevel,
                                   entranceFee);
            }
            else if (!levelRequirementMet)
            {
                ImGui::TextWrapped("You need to be at least level %d to enter this dungeon.",
                                   minPlayerLevel);
            }
            else if (!goldRequirementMet)
            {
                ImGui::TextWrapped("You need %d gold to enter this dungeon.", entranceFee);
            }

            ImGui::PopStyleColor();
        }
        else
        {
            // Show success message when all requirements are met
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));  // Green text
            ImGui::TextWrapped("You meet all requirements to enter this dungeon. Ready for the "
                               "challenge?");
            ImGui::PopStyleColor();
        }

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Buttons at the bottom
        float contentWidth = ImGui::GetContentRegionAvail().x;

        if (allRequirementsMet)
        {
            // Both "Go" and "Cancel" buttons
            float buttonWidth = 120.0f;
            float startPos    = (contentWidth - (buttonWidth * 2)) / 2.0f + 12.0f;

            ImGui::SetCursorPosX(startPos);

            // Go button (green)
            if (RenderStyledButton("Enter", ImVec2(buttonWidth, 40), true))
            {
                if (character)
                    character->SetControlledByPlayer(false);

                // Deduct entrance fee
                if (playerChar)
                    playerChar->AddGold(-entranceFee);

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

            // Cancel button (grey)
            if (RenderStyledButton("Cancel", ImVec2(buttonWidth, 40), false))
            {
                if (character)
                    character->SetControlledByPlayer(true);
                m_showPortalEnterModal = false;
                m_currentPortalId      = -1;
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            // Only "Back" button centered
            float buttonWidth = 120.0f;
            float startPos    = (contentWidth - buttonWidth) / 2.0f + 12.0f;

            ImGui::SetCursorPosX(startPos);

            // Back button (grey)
            if (RenderStyledButton("Back", ImVec2(buttonWidth, 40), false))
            {
                if (character)
                    character->SetControlledByPlayer(true);
                m_showPortalEnterModal = false;
                m_currentPortalId      = -1;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    else
    {
        // If the popup isn't open (was closed), reset the flag
        m_showPortalEnterModal = false;
    }

    // Pop the modal window styles
    PopModalStyle();
}

void WorldState::RenderShopEnterModal(AnimatedUnit* character)
{
    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(320, 160));

    // Apply standard modal styling
    PushModalStyle(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    ImGui::OpenPopup("Enter Shop");

    if (ImGui::BeginPopupModal("Enter Shop",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav |
                                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
    {
        const float sectionSpacing = 3.0f;

        // Shop title header
        ImGui::SetWindowFontScale(1.3f);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Chiruzu's Shop");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Shop description
        ImGui::TextWrapped("Would you like to enter the shop?");

        ImGui::Dummy(ImVec2(0, sectionSpacing + 1.0f));

        // Buttons at the bottom
        float contentWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth  = 120.0f;
        float startPos     = (contentWidth - (buttonWidth * 2)) / 2.0f + 12.0f;

        ImGui::SetCursorPosX(startPos);

        // Yes button (green)
        if (RenderStyledButton("Yes", ImVec2(buttonWidth, 40), true))
        {
            if (character)
                character->SetControlledByPlayer(true);

            m_showShopEnterModal = false;
            ImGui::CloseCurrentPopup();
            GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(false);

            std::vector<std::string> textures     = {"shop_dialog_1"};
            std::vector<std::string> descriptions = {"Welcome to my lovely shop, I'm Chiruzu!"};

            // Create DialogState and pass it to StateManager
            m_pendingStateChange = StateChange {
                StateAction::PUSH,
                std::make_unique<DialogState>(GetContext(),
                                              textures,      // Texture for shop keeper
                                              descriptions,  // Dialog text
                                              1,  // dialogCount - indicating 1 dialog to show
                                              std::make_unique<ShopState>(GetContext())  // Destination
                                                                                         // after
                                                                                         // dialog
                                              )};
        }

        ImGui::SameLine();

        // Cancel button (grey)
        if (RenderStyledButton("Cancel", ImVec2(buttonWidth, 40), false))
        {
            if (character)
                character->SetControlledByPlayer(true);
            m_showShopEnterModal = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Pop the modal window styles
    PopModalStyle();
}

void WorldState::RenderErrorModal()
{
    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    // Style with red border for error
    PushModalStyle(ImVec4(0.7f, 0.2f, 0.2f, 1.0f));

    ImGui::OpenPopup("Error");
    m_showErrorPopup = false;  // prevent reopening every frame

    if (ImGui::BeginPopupModal("Error",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Error icon or title could go here
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));  // Red text
        ImGui::Text("%s", m_errorMessage.c_str());
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10.0f));  // Add some space
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10.0f));  // Add some space

        float buttonWidth = 120.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f;
        ImGui::SetCursorPosX(buttonPosX);

        // OK button
        if (RenderStyledButton("OK", ImVec2(buttonWidth, 35), false))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Pop the modal window styles
    PopModalStyle();
}

void WorldState::RenderExitConfirmationModal()
{
    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(320, 180));

    // Apply standard modal styling
    PushModalStyle(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    ImGui::OpenPopup("Exit Confirmation");
    // REMOVE THIS LINE: m_showExitPopup = false;  // prevent reopening every frame

    if (ImGui::BeginPopupModal("Exit Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar))
    {
        const float sectionSpacing = 3.0f;

        // Title header
        ImGui::SetWindowFontScale(1.2f);
        ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Exit Confirmation");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Dummy(ImVec2(0, sectionSpacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

        // Warning message
        ImGui::TextWrapped("Do you want to save your progress before exiting?");

        ImGui::Dummy(ImVec2(0, sectionSpacing * 1.5f));

        // Buttons at the bottom
        float contentWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth  = 90.0f;  // Reduced button width to fit 3 buttons
        float startPos     = (contentWidth - (buttonWidth * 3) - (10.0f * 2)) / 2.0f + 15.0f;

        ImGui::SetCursorPosX(startPos);

        // Custom green Yes button for exit
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));  // Green
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

        if (ImGui::Button("Yes", ImVec2(buttonWidth, 40)))
        {
            ImGui::CloseCurrentPopup();
            m_showExitPopup = false;  // Reset flag when user clicks a button

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

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::SameLine(0, 10.0f);  // Add spacing between buttons

        // Custom Red NO button
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));  // Dark red
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));

        if (ImGui::Button("No", ImVec2(buttonWidth, 40)))
        {
            ImGui::CloseCurrentPopup();
            m_showExitPopup = false;  // Reset flag when user clicks a button

            m_pendingStateChange = StateChange {StateAction::POP};
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::SameLine(0, 10.0f);  // Add spacing between buttons

        // Gray Cancel button - darker gray
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Darker Gray
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));

        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 40)))
        {
            ImGui::CloseCurrentPopup();
            m_showExitPopup = false;  // Reset flag when user clicks a button
            // No m_pendingStateChange modification here, as requested
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::EndPopup();
    }
    else
    {
        // If the popup isn't open (was closed), reset the flag
        m_showExitPopup = false;
    }

    // Pop the modal window styles
    PopModalStyle();
}

void WorldState::RenderPlayerInfo()
{
    // Get player character
    Character* character =
        GetContext().GetUnitManager()->GetUnitOfType<Character>(GetContext().GetCharacterId());
    if (!character)
        return;

    // Set window position (bottom right)
    const ImGuiViewport* viewport     = ImGui::GetMainViewport();
    float                barWidth     = 200.0f;
    float                windowHeight = 110.0f;  // Reduced height
    ImVec2               windowPos(viewport->WorkPos.x + viewport->WorkSize.x - barWidth - 20.0f,
                     viewport->WorkPos.y + viewport->WorkSize.y - windowHeight - 10.0f);  // Lower
                                                                                          // position
    ImVec2 windowSize(barWidth, windowHeight);

    // Set window position and size
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    // Window flags: no decorations, no interactions
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav;

    // Custom ImGui styling for the player info panel
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.7f, 0.6f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.05f, 0.05f, 0.05f, 0.8f));

    if (ImGui::Begin("##PlayerInfo", nullptr, flags))
    {
        // Player name with custom styling
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));  // Gold text

        // Center the name
        auto nameSize = ImGui::CalcTextSize(character->GetName().c_str());
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - nameSize.x) * 0.5f);
        ImGui::Text("%s", character->GetName().c_str());

        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.7f, 0.6f, 0.3f, 1.0f));  // Gold color
        ImGui::Separator();
        ImGui::PopStyleColor();

        ImGui::Spacing();

        // Player level
        ImGui::Text("Level: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%d", character->GetLevel());

        // Player experience with the provided formula
        int currentExp = character->GetExp();
        int level      = character->GetLevel();
        int expNeeded  = std::pow(level, 1.7) + 100;  // Using the formula you provided

        // Adjust padding to start the progress bar closer to the left edge
        float padX = 5.0f;  // Reduced from 5%
        ImGui::SetCursorPosX(padX);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() - padX * 2);

        // Experience bar
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);  // Increase rounding for frames

        ImVec4 expBarColor   = ImVec4(1.0f, 0.8f, 0.2f, 0.8f);   // Gold for exp bar
        ImVec4 expBarBgColor = ImVec4(0.3f, 0.25f, 0.1f, 0.6f);  // Darker gold for background

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, expBarColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, expBarBgColor);  // Add matching background color

        // No need to create a separate string, we'll use the one we already have
        // Convert to C-style string for consistency with your example
        char expOverlay[32];
        snprintf(expOverlay, sizeof(expOverlay), "%d/%d", currentExp, expNeeded);

        // Progress bar with text overlay directly inside using the empty string trick
        ImGui::ProgressBar(static_cast<float>(currentExp) /
                               static_cast<float>(expNeeded > 0 ? expNeeded : 1),
                           ImVec2(-1, 20),
                           "");  // Slightly taller for better rounding

        // Position text manually over the progress bar
        auto  expTextSize = ImGui::CalcTextSize(expOverlay);
        float textPosX    = (ImGui::GetWindowWidth() - expTextSize.x) * 0.5f;
        float textPosY    = ImGui::GetCursorPosY() - 20.0f;  // Position text over the exp bar

        ImGui::SetCursorPos(ImVec2(textPosX, textPosY));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // Dark text color
        ImGui::Text("%s", expOverlay);
        ImGui::PopStyleColor();

        // Pop the progress bar colors
        ImGui::PopStyleColor(2);

        ImGui::PopStyleVar();  // Pop the frame rounding style

        ImGui::Spacing();
        ImGui::Spacing();

        // Player gold
        ImGui::Text("Gold: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%d", character->GetGold());
    }
    ImGui::End();

    // Pop styles
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(5);
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
    m_backsound.pause();
    m_lastPosition = GetContext()
                         .GetUnitManager()
                         ->GetUnitOfType<AnimatedUnit>(GetContext().GetCharacterId())
                         ->GetPosition();
}

void WorldState::Resume()
{
    m_backsound.play();
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
        animatedCharacter->SetShowUI(false);
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
    m_backsound.stop();
    GetContext().GetUnitManager()->Clear();
}
