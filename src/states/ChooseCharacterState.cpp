#include "states/ChooseCharacterState.hpp"

// Constructor
ChooseCharacterState::ChooseCharacterState(GameContext& context)
    : State(context),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_squareButtonTexture(GetContext().GetResourceManager()->GetTexture("square_button")),
      m_exitButton(m_squareButtonTexture, {32.f, 32.f}, {0.5f, 0.5f}),
      m_confirmButton(m_buttonTexture,  // at the center of screen
                      {0.f, 0.f}),      // set later
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_boldFont(GetContext().GetResourceManager()->GetFont("main_bold_font")),
      m_menuTitle(m_boldFont),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("choose_character_menu_"
                                                                        "background")),
      m_backgroundSprite(m_backgroundTexture),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Choose Character State");
}

void ChooseCharacterState::Init()
{
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

    // title
    m_menuTitle.setString("Choose Character");

    m_menuTitle.setFillColor({255, 255, 255, 144});
    m_menuTitle.setCharacterSize(64);

    m_menuTitle.setOutlineColor(sf::Color::Black);
    m_menuTitle.setOutlineThickness(8.f);

    // Set origin to the center of the text bounds
    sf::FloatRect textBounds = m_menuTitle.getLocalBounds();
    m_menuTitle.setOrigin(textBounds.getCenter());
    m_menuTitle.setPosition({windowSize.x / 2.f, windowSize.y * 0.28f});

    m_exitButton.setText("X", m_font, 24);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    m_confirmButton.setText("Confirm Character", m_font, 30);
    m_confirmButton.setHoverSound(m_buttonHoverSound);
    m_confirmButton.setClickSound(m_buttonClickSound);
    m_confirmButton.setActive(false);

    // Set button callbacks
    m_exitButton.setOnClickCallback([this]() {
        GetContext().GetUnitManager()->Clear();
        m_pendingStateChange = StateChange {StateAction::POP};
    });

    m_confirmButton.setOnClickCallback([this]() { m_showNameModal = true; });

    m_confirmButton.setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});

    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    std::vector<std::vector<bool>> grid = {
        {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0},
        {1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0},
        {1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0},
        {1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    GetContext().GetNavigationGrid()->SetGrid(grid);
    NavigationGrid* navGrid = GetContext().GetNavigationGrid();

    auto fighter =
        std::make_unique<Fighter>("Fighter",
                                  sf::Vector2f(windowSize.x * (1.0f / 6), windowSize.y * 0.76f),
                                  *navGrid,
                                  false,
                                  GetContext());
    auto mage = std::make_unique<Mage>("Mage",
                                       sf::Vector2f(windowSize.x * (2.0f / 6), windowSize.y * 0.76f),
                                       *navGrid,
                                       false,
                                       GetContext());

    auto assassin =
        std::make_unique<Assassin>("Assassin",
                                   sf::Vector2f(windowSize.x * (3.0f / 6), windowSize.y * 0.76f),
                                   *navGrid,
                                   false,
                                   GetContext());
    auto necromancer =
        std::make_unique<Necromancer>("Necromancer",
                                      sf::Vector2f(windowSize.x * (4.0f / 6), windowSize.y * 0.76f),
                                      *navGrid,
                                      false,
                                      GetContext());
    auto berseker =
        std::make_unique<Berseker>("Berseker",
                                   sf::Vector2f(windowSize.x * (5.0f / 6), windowSize.y * 0.76f),
                                   *navGrid,
                                   false,
                                   GetContext());

    fighter->SetScale(10.f, 10.f);
    mage->SetScale(10.f, 10.f);
    assassin->SetScale(10.f, 10.f);
    necromancer->SetScale(10.f, 10.f);
    berseker->SetScale(10.f, 10.f);

    m_fighterId     = fighter->GetId();
    m_mageId        = mage->GetId();
    m_assassinId    = assassin->GetId();
    m_necromancerId = necromancer->GetId();
    m_bersekerId    = berseker->GetId();

    GetContext().GetUnitManager()->AddUnit(std::move(fighter));
    GetContext().GetUnitManager()->AddUnit(std::move(mage));
    GetContext().GetUnitManager()->AddUnit(std::move(assassin));
    GetContext().GetUnitManager()->AddUnit(std::move(necromancer));
    GetContext().GetUnitManager()->AddUnit(std::move(berseker));
}

State::StateChange ChooseCharacterState::ProcessEvent(const sf::Event& event)
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

State::StateChange ChooseCharacterState::Update(const sf::Time& dt)
{
    sf::RenderWindow* window = GetContext().GetWindow();

    // The confirm button should only be active if a character is selected
    // AND no modals are currently being displayed
    bool isNameModalActive = ImGui::IsPopupOpen("Enter Username");

    // Only enable the confirm button if a character is selected and no modals are open
    m_confirmButton.setActive(m_selectedCharacterId != -1 && !isNameModalActive);

    // Only update buttons if no modals are open
    if (!isNameModalActive)
    {
        m_exitButton.update(*window);
        m_confirmButton.update(*window);
    }

    // Check if we have a pending state change from a button callback
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        return std::move(m_pendingStateChange);
    }

    return StateChange {StateAction::NONE};
}

void ChooseCharacterState::Draw(sf::RenderWindow& window)
{
    window.clear({20, 20, 20});

    window.draw(m_backgroundSprite);
    window.draw(m_menuTitle);

    m_exitButton.draw(window);
    m_confirmButton.draw(window);
}

void ChooseCharacterState::RenderCharacterSelectionUI()
{
    sf::Vector2u     windowSize   = GetContext().GetWindow()->getSize();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus;  // ← key flag

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)windowSize.x, (float)windowSize.y));

    if (ImGui::Begin("Character Selection", nullptr, window_flags))
    {
        // — styles for checkboxes —
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.3f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.35f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 0.8f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.9f, 0.9f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));

        const char*  characterNames[] = {"Fighter", "Mage", "Assassin", "Necromancer", "Berserker"};
        unsigned int characterIds[]   = {
            m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        // — draw the five checkboxes under each sprite —
        for (int i = 0; i < 5; i++)
        {
            float px = windowSize.x * ((i + 1.0f) / 6.0f) - 6.0f;
            float py = windowSize.y * 0.85f;
            float cb = ImGui::GetFrameHeight();

            ImGui::SetCursorPos(ImVec2(px - cb / 2.0f, py));
            std::string lbl   = "##" + std::string(characterNames[i]);
            bool        isSel = (m_selectedCharacterId == characterIds[i]);

            if (ImGui::Checkbox(lbl.c_str(), &isSel))
            {
                if (isSel)
                {
                    m_selectedCharacterId = characterIds[i];
                    if (auto* u = dynamic_cast<AnimatedUnit*>(
                            GetContext().GetUnitManager()->GetUnit(characterIds[i])))
                        u->PlayAnimation(UnitAnimationType::JUMP, nullptr);
                }
                else if (m_selectedCharacterId == (int)characterIds[i])
                {
                    m_selectedCharacterId = -1;
                }
            }

            // label under checkbox
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
            ImGui::SetWindowFontScale(1.2f);
            float tw = ImGui::CalcTextSize(characterNames[i]).x;
            ImGui::SetCursorPos(ImVec2(px - tw / 2.0f, py + cb + 10.0f));
            ImGui::Text("%s", characterNames[i]);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
        }

        // — Info "?" button next to selected character —
        if (m_selectedCharacterId != -1)
        {
            int selIdx = -1;
            for (int i = 0; i < 5; ++i)
                if (characterIds[i] == m_selectedCharacterId)
                    selIdx = i;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.6f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.7f, 0.9f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.5f, 1.0f));

            float px = windowSize.x * ((selIdx + 1.0f) / 6.0f);
            float py = windowSize.y * 0.85f;
            ImGui::SetCursorPos(ImVec2(px + 25.0f, py));

            if (ImGui::Button("?", ImVec2(30, 30)))
            {
                // Toggle character info window
                if (m_showCharacterInfo)
                {
                    m_showCharacterInfo       = false;
                    m_justOpenedCharacterInfo = false;
                }
                else
                {
                    m_showCharacterInfo       = true;
                    m_justOpenedCharacterInfo = true;
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Show character information");

            ImGui::PopStyleColor(3);
        }

        ImGui::PopFont();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);
    }
    ImGui::End();
}

void ChooseCharacterState::RenderCharacterInfoWindow()
{
    // Only render if character is selected and info window should be shown
    if (m_selectedCharacterId == -1 || !m_showCharacterInfo)
        return;

    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

    if (m_justOpenedCharacterInfo)
    {
        ImGui::SetNextWindowFocus();
        m_justOpenedCharacterInfo = false;
    }

    ImGuiWindowFlags infoFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoSavedSettings;

    const char* characterNames[] = {"Fighter", "Mage", "Assassin", "Necromancer", "Berserker"};
    unsigned int characterIds[] = {m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};

    int selIdx = -1;
    for (int i = 0; i < 5; ++i)
        if (characterIds[i] == m_selectedCharacterId)
            selIdx = i;

    float infoX = windowSize.x * ((selIdx + 1.0f) / 6.0f) - 150.0f;
    float infoY = windowSize.y * 0.4f;
    ImGui::SetNextWindowPos(ImVec2(infoX, infoY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Once);

    std::string title = std::string(characterNames[selIdx]) + " Info";
    if (ImGui::Begin(title.c_str(), &m_showCharacterInfo, infoFlags))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        ImGui::TextColored(
            ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Character Class: %s", characterNames[selIdx]);
        ImGui::Separator();

        switch (selIdx)
        {
            case 0:  // Fighter
                ImGui::Text("Special: Blocking");
                ImGui::Separator();
                ImGui::TextWrapped("A powerful melee fighter with high physical damage and "
                                   "durability.");
                break;
            case 1:  // Mage
                ImGui::Text("Special: Extra mana");
                ImGui::Separator();
                ImGui::TextWrapped("A master of arcane arts with powerful area-of-effect "
                                   "spells.");
                break;
            case 2:  // Assassin
                ImGui::Text("Special: High critical chance");
                ImGui::Separator();
                ImGui::TextWrapped("A swift dealer of deadly strikes with high critical damage "
                                   "potential.");
                break;
            case 3:  // Necromancer
                ImGui::Text("Special: Can summon undead zombie");
                ImGui::Separator();
                ImGui::TextWrapped("Commands the powers of death to summon zombies and drain "
                                   "life force.");
                break;
            case 4:  // Berserker
                ImGui::Text("Special: Rage buff");
                ImGui::Separator();
                ImGui::TextWrapped("A fearsome warrior whose power grows with rage and battle "
                                   "damage.");
                break;
        }

        ImGui::Separator();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120) * 0.5f);
        if (ImGui::Button("Close", ImVec2(120, 30)))
            m_showCharacterInfo = false;

        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void ChooseCharacterState::RenderNameInputModal()
{
    constexpr char  PopupId[]     = "Enter Username";
    constexpr float ButtonWidth   = 100.0f;
    constexpr float ButtonHeight  = 40.0f;
    constexpr float ButtonSpacing = 20.0f;

    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 0));  // Width fixed, height auto

    // Apply modal styling directly
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    // Open popup if flag is set
    if (m_showNameModal)
    {
        ImGui::OpenPopup(PopupId);
        m_showNameModal = false;  // Reset flag to prevent reopening every frame
    }

    if (!ImGui::BeginPopupModal(PopupId,
                                nullptr,
                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor(3);
        return;
    }

    const float sectionSpacing = 3.0f;

    // Title header with blue text
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.8f, 1.0f), "Enter Character Name");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

    // Instruction text with wrapping
    ImGui::TextWrapped("Please enter a name for your character:");
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Input field styling
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    // Make the input field fill the width
    float availableWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetNextItemWidth(availableWidth);
    ImGui::InputText("##username", m_userNameBuffer, sizeof(m_userNameBuffer));

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::Dummy(ImVec2(0, sectionSpacing * 1.5f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Check if the field is empty
    bool hasName = (m_userNameBuffer[0] != '\0');

    // Buttons at the bottom - centered
    float contentWidth      = ImGui::GetContentRegionAvail().x;
    float totalButtonsWidth = (ButtonWidth * 2) + ButtonSpacing;
    float startPos          = (contentWidth - totalButtonsWidth) / 2.0f;
    ImGui::SetCursorPosX(startPos);

    // Finish button (blue)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    // Change button style based on whether input is provided
    if (!hasName)
    {
        // Disabled style with reduced alpha and grayed-out colors
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.3f, 0.5f, 0.5f));  // Muted blue
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.3f, 0.5f, 0.5f));  // No hover
                                                                                        // effect
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.3f, 0.5f, 0.5f));   // No active
                                                                                        // effect
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.3f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));  // Dimmed text
    }
    else
    {
        // Normal enabled style
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));  // Blue
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    if (ImGui::Button("Finish", ImVec2(ButtonWidth, ButtonHeight)) && hasName)
    {
        GetContext().SetCharacterId(m_selectedCharacterId);

        unsigned int characterIds[] = {
            m_fighterId, m_mageId, m_assassinId, m_necromancerId, m_bersekerId};
        for (const auto& id : characterIds)
        {
            if (m_selectedCharacterId != id)
                GetContext().GetUnitManager()->RemoveUnit(id);
        }

        Character* character =
            GetContext().GetUnitManager()->GetUnitOfType<Character>(m_selectedCharacterId);
        character->SetName(m_userNameBuffer);

        m_pendingStateChange =
            StateChange {StateAction::REPLACE, std::make_unique<WorldState>(GetContext())};
        ImGui::CloseCurrentPopup();
    }

    // Display tooltip on hover when button is disabled
    if (!hasName && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Please enter a character name");
    }

    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);

    ImGui::SameLine(0, ButtonSpacing);

    // Cancel button (gray)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Dark Gray
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));

    if (ImGui::Button("Cancel", ImVec2(ButtonWidth, ButtonHeight)))
    {
        ImGui::CloseCurrentPopup();
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::EndPopup();

    // Pop the modal window styles
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(3);
}

// Main RenderUI function that calls the separated functions
void ChooseCharacterState::RenderUI()
{
    // Render all UI components
    RenderCharacterSelectionUI();
    RenderCharacterInfoWindow();
    RenderNameInputModal();
}

void ChooseCharacterState::Exit() {}