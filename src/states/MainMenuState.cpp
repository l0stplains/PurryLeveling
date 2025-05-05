#include "states/MainMenuState.hpp"

// Constructor
MainMenuState::MainMenuState(GameContext& context)
    : State(context),
      m_backsound(GetContext().GetResourceManager()->GetSoundBuffer("menu_backsound")),
      m_buttonHoverSound(GetContext().GetResourceManager()->GetSoundBuffer("button_hover")),
      m_buttonClickSound(GetContext().GetResourceManager()->GetSoundBuffer("button_click")),
      m_backgroundTexture(GetContext().GetResourceManager()->GetTexture("main_menu_background")),
      m_logoTexture(GetContext().GetResourceManager()->GetTexture("main_logo")),
      m_logoShadowTexture(GetContext().GetResourceManager()->GetTexture("main_logo_shadow")),
      m_buttonTexture(GetContext().GetResourceManager()->GetTexture("main_menu_button")),
      m_backgroundSprite(m_backgroundTexture),
      m_logoSprite(m_logoTexture),
      m_logoShadowSprite(m_logoShadowTexture),
      m_newGameButton(
          m_buttonTexture,
          {90.f + m_buttonTexture.getSize().x / 2.f,
           200.f + m_buttonTexture.getSize().y + 32.f + m_buttonTexture.getSize().y / 2.f}),
      m_loadButton(
          m_buttonTexture,
          {90.f + m_buttonTexture.getSize().x / 2.f,
           200.f + 2 * (m_buttonTexture.getSize().y + 32.f) + m_buttonTexture.getSize().y / 2.f}),
      m_exitButton(
          m_buttonTexture,
          {90.f + m_buttonTexture.getSize().x / 2.f,
           200.f + 3 * (m_buttonTexture.getSize().y + 32.f) + m_buttonTexture.getSize().y / 2.f}),
      m_pendingStateChange({StateAction::NONE})
{
    SetName("Main Menu State");
}

void MainMenuState::Init()
{
    // Background music setup
    m_backsound.setLooping(true);
    m_backsound.setVolume(50);
    m_backsound.play();

    // Load font
    m_font = GetContext().GetResourceManager()->GetFont("main_font");

    // Background setup
    m_backgroundSprite.setOrigin({0, 0});
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();
    m_backgroundSprite.setScale({static_cast<float>(windowSize.x) / m_backgroundTexture.getSize().x,
                                 static_cast<float>(windowSize.y) / m_backgroundTexture.getSize().y});
    m_backgroundSprite.setPosition({0, 0});

    // Logo setup
    m_logoShadowSprite.setOrigin({m_logoShadowTexture.getSize().x, 0});
    m_logoShadowSprite.setScale({320.f / m_logoShadowTexture.getSize().x,
                                 320.f / m_logoShadowTexture.getSize().y});  // Scale to 320x320
    m_logoShadowSprite.setPosition({windowSize.x - 64.f, 32.f});             // 32px margin

    m_logoSprite.setOrigin({m_logoTexture.getSize().x, 0});
    m_logoSprite.setScale({320.f / m_logoTexture.getSize().x,
                           320.f / m_logoTexture.getSize().y});  // Scale to 320x320
    m_logoSprite.setPosition({windowSize.x - 64.f, 32.f});       // 32px margin

    // Button setup
    m_newGameButton.setText("New Game", m_font, 32);
    m_loadButton.setText("Load Game", m_font, 32);
    m_exitButton.setText("Exit", m_font, 32);

    // Set button sounds
    m_newGameButton.setHoverSound(m_buttonHoverSound);
    m_newGameButton.setClickSound(m_buttonClickSound);
    m_loadButton.setHoverSound(m_buttonHoverSound);
    m_loadButton.setClickSound(m_buttonClickSound);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    // Set button callbacks

    m_newGameButton.setOnClickCallback([this]() {
        std::cout << "New Game button clicked" << std::endl;
        // change this state to not rendered
        m_showSaveNamePopup = true;
        // m_pendingStateChange = StateChange{StateAction::PUSH, nullptr};
    });

    m_loadButton.setOnClickCallback([this]() {
        m_showFileDialog = true;
        /*
        IGFD::FileDialogConfig config;
        config.path              = ".";  // starting folder
        config.countSelectionMax = 1;    // just one selection (optional)
        config.flags             = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                       ImGuiFileDialogFlags_DisableCreateDirectoryButton;

        config.sidePaneWidth = 250.0f;  // if you use a side panel

        config.userDatas = nullptr;  // optional user data

        // Open file dialog with default path
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFolderDlgKey",    // dialog key
                                                "Select a Save Folder",  // window title
                                                nullptr,  // <— nullptr filter = directory chooser
                                                          // :contentReference[oaicite:0]{index=0}
                                                config    // flags
        );
        */
    });

    m_exitButton.setOnClickCallback([this]() { m_showExitPopup = true; });

    // Disable continue button if no save exists (example)
    bool hasSaveGame = false;  // This would be your save detection logic
}

State::StateChange MainMenuState::ProcessEvent(const sf::Event& event)
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

State::StateChange MainMenuState::Update(const sf::Time& dt)
{
    sf::RenderWindow* window = GetContext().GetWindow();

    // Update all buttons
    if (!m_showFileDialog && !m_showErrorPopup && !m_showExitPopup)
    {
        m_newGameButton.update(*window);
        m_loadButton.update(*window);
        m_exitButton.update(*window);
    }

    // Check if we have a pending state change from a button callback
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        return std::move(m_pendingStateChange);
    }

    return StateChange {StateAction::NONE};
}

void MainMenuState::Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(0, 0, 0));

    // Draw background
    window.draw(m_backgroundSprite);

    // Draw logo
    window.draw(m_logoShadowSprite);
    window.draw(m_logoSprite);

    // Draw buttons
    m_newGameButton.draw(window);
    m_loadButton.draw(window);
    m_exitButton.draw(window);
}

void MainMenuState::RenderUI()
{
    renderNewSaveModal();
    renderLoadSaveModal();
    renderErrorModal();
    renderExitConfirmationModal();
}

void MainMenuState::renderNewSaveModal()
{
    constexpr char  PopupId[]     = "Enter Save Name";
    constexpr float ButtonWidth   = 90.0f;
    constexpr float ButtonHeight  = 40.0f;
    constexpr float ButtonSpacing = 10.0f;

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

    if (m_showSaveNamePopup)
    {
        ImGui::OpenPopup(PopupId);
        m_showSaveNamePopup = false;
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

    // Title header with green text
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Create New Game");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

    // Instruction text with wrapping
    ImGui::TextWrapped("Please enter a name for your save folder:");
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Input field styling
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    // Make the input field fill the width with a specific size
    float availableWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetNextItemWidth(availableWidth);
    ImGui::InputText("##SaveName", m_saveNameBuf, sizeof(m_saveNameBuf));

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::Dummy(ImVec2(0, sectionSpacing * 1.5f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Lambda to perform the actual save-confirm action
    auto confirmSave = [&]() {
        const auto fullPath = std::filesystem::path("data") / m_saveNameBuf;
        if (std::filesystem::exists(fullPath))
        {
            showError("Save folder already exists: " + fullPath.string());
        }
        else
        {
            GetContext().SetCurrentFolderName(m_saveNameBuf);
            GetContext().SetFirstSaveState(true);
            ImGui::CloseCurrentPopup();
            parseNonPlayerConfig("resources/config");
            m_pendingStateChange =
                StateChange {StateAction::PUSH, std::make_unique<ChooseCharacterState>(GetContext())};
        }
    };

    // Check if the field is empty
    bool hasName = (m_saveNameBuf[0] != '\0');

    // Buttons at the bottom - centered
    float contentWidth      = ImGui::GetContentRegionAvail().x;
    float totalButtonsWidth = (ButtonWidth * 2) + ButtonSpacing;
    float startPos          = (contentWidth - totalButtonsWidth) / 2.0f + 12.0f;
    ImGui::SetCursorPosX(startPos);

    // OK button (green)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    // Change button style based on whether input is provided
    if (!hasName)
    {
        // Disabled style with reduced alpha and grayed-out colors
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.2f, 0.5f));  // Muted green
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 0.2f, 0.5f));  // No hover
                                                                                        // effect
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.4f, 0.2f, 0.5f));   // No active
                                                                                        // effect
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.4f, 0.2f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));  // Dimmed text
    }
    else
    {
        // Normal enabled style
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));  // Green
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    if (ImGui::Button("OK", ImVec2(ButtonWidth, ButtonHeight)) && hasName)
    {
        confirmSave();
    }

    // Display tooltip on hover when button is disabled
    if (!hasName && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Please enter a save name");
    }

    ImGui::PopStyleColor(5);  // Pop all 5 style colors
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

void MainMenuState::renderLoadSaveModal()
{
    constexpr char  PopupId[]      = "Load Game";
    constexpr float CancelBtnWidth = 100.0f;
    constexpr float ButtonHeight   = 40.0f;

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

    // 1) Trigger popup
    if (m_showFileDialog)
    {
        ImGui::OpenPopup(PopupId);
        m_showFileDialog = false;
    }

    // 2) Begin modal with improved styling
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

    // Title header with golden text
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Load Saved Game");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

    // Instruction text with wrapping
    ImGui::TextWrapped("Select a save folder to continue your adventure:");
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Create a child window with fixed height for scrollable save list only
    // Calculate a reasonable fixed height (e.g., enough for 5-6 items)
    float fixedChildHeight = 150.0f;  // Fixed height for the scrollable area
    ImGui::BeginChild(
        "SaveList", ImVec2(0, fixedChildHeight), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // 3) List each subfolder in "data/" as a selectable entry with improved styling
    for (const auto& entry : std::filesystem::directory_iterator("data"))
    {
        if (!entry.is_directory())
            continue;

        const std::string folderName = entry.path().filename().string();

        // Style for save folder items
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.6f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.5f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.6f, 0.8f, 0.7f));

        if (ImGui::Selectable(folderName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
        {
            // Immediately attempt to load when clicked
            m_selectedFolder = folderName;
            try
            {
                m_selectedFolder = "data/" + m_selectedFolder;
                validateFolder(m_selectedFolder);

                // Strip any leading "data/" if present
                constexpr char dataPrefix[] = "data/";
                if (m_selectedFolder.rfind(dataPrefix, 0) == 0)
                    m_selectedFolder.erase(0, sizeof(dataPrefix) - 1);

                GetContext().SetCurrentFolderName(m_selectedFolder);
                m_pendingStateChange =
                    StateChange {StateAction::PUSH, std::make_unique<WorldState>(GetContext())};
                std::cout << "Selected valid save folder: " << m_selectedFolder << std::endl;
                ImGui::CloseCurrentPopup();
            }
            catch (const FileNotFoundException& e)
            {
                showError("File not found: " + e.getFilename() +
                          (e.getErrorMessage().empty() ? "" : " - " + e.getErrorMessage()));
            }
            catch (const LineTooShortException& e)
            {
                showError("Line too short in file: " + e.getFilename() +
                          (e.getErrorMessage().empty() ? "" : " - " + e.getErrorMessage()));
            }
            catch (const InvalidFormatException& e)
            {
                showError("Invalid format in file: " + e.getFilename() +
                          (e.getErrorMessage().empty() ? "" : " - " + e.getErrorMessage()));
            }
            catch (const ResourceNotFoundException& e)
            {
                showError("Resource not found: " + e.getFilename() +
                          (e.getErrorMessage().empty() ? "" : " - " + e.getErrorMessage()));
            }
            catch (const MissingFileException& e)
            {
                showError("Missing file: " + e.getFilename() +
                          (e.getErrorMessage().empty() ? "" : " - " + e.getErrorMessage()));
            }
            catch (const InvalidCharacterTypeException& e)
            {
                showError(std::string("Invalid character type: ") + e.what());
            }
            catch (const std::exception& e)
            {
                showError(std::string("An error occurred: ") + e.what());
            }
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::EndChild();

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // 4) Center the Cancel button
    float contentWidth = ImGui::GetContentRegionAvail().x;
    float startPos     = (contentWidth - CancelBtnWidth) / 2.0f;
    ImGui::SetCursorPosX(startPos);

    // Cancel button styling (dark gray)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Dark Gray
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));

    if (ImGui::Button("Cancel", ImVec2(CancelBtnWidth, ButtonHeight)))
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

void MainMenuState::renderErrorModal()
{
    constexpr char  PopupId[]    = "Error";
    constexpr float ButtonWidth  = 100.0f;
    constexpr float ButtonHeight = 40.0f;

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

    if (m_showErrorPopup)
    {
        ImGui::OpenPopup(PopupId);
        m_showErrorPopup = false;
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

    // Title header with red error text
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Error");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

    // Error message with wrapping
    ImGui::TextWrapped("%s", m_errorMessage.c_str());

    ImGui::Dummy(ImVec2(0, sectionSpacing * 1.5f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Center the OK button
    float contentWidth = ImGui::GetContentRegionAvail().x;
    float startPos     = (contentWidth - ButtonWidth) / 2.0f;
    ImGui::SetCursorPosX(startPos);

    // OK button styling (dark gray)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));  // Dark Gray
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));

    if (ImGui::Button("OK", ImVec2(ButtonWidth, ButtonHeight)))
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

void MainMenuState::renderExitConfirmationModal()
{
    constexpr char  PopupId[]     = "Exit Confirmation";
    constexpr float ButtonWidth   = 90.0f;  // Reduced width to fit 3 buttons
    constexpr float ButtonHeight  = 40.0f;
    constexpr float ButtonSpacing = 10.0f;

    // Center the modal window
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(320, 180));  // Fixed size like in WorldState

    // Apply modal styling directly
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    if (m_showExitPopup)
    {
        ImGui::OpenPopup(PopupId);
        m_showExitPopup = false;
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

    // Title header with golden text
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Exit Confirmation");
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Dummy(ImVec2(0, sectionSpacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing * 0.75f));

    // Warning message
    ImGui::TextWrapped("Do you want to exit?");

    ImGui::Dummy(ImVec2(0, sectionSpacing * 1.5f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, sectionSpacing));

    // Buttons at the bottom - centered
    float contentWidth      = ImGui::GetContentRegionAvail().x;
    float totalButtonsWidth = (ButtonWidth * 2) + (ButtonSpacing * 2);
    float startPos          = (contentWidth - totalButtonsWidth) / 2.0f + 12.0f;
    ImGui::SetCursorPosX(startPos);

    // Yes button (green)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));  // Green
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

    if (ImGui::Button("Yes", ImVec2(ButtonWidth, ButtonHeight)))
    {
        ImGui::CloseCurrentPopup();
        GetContext().GetWindow()->close();
        m_pendingStateChange = StateChange {};
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::SameLine(0, ButtonSpacing);

    // No button (red)
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));  // Dark red
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));

    if (ImGui::Button("No", ImVec2(ButtonWidth, ButtonHeight)))
    {
        ImGui::CloseCurrentPopup();
    }

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);

    ImGui::SameLine(0, ButtonSpacing);

    ImGui::EndPopup();

    // Pop the modal window styles
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor(3);
}

void MainMenuState::validateFolder(const std::string& folderPath)
{
    checkRequiredFiles(folderPath);
    parseNonPlayerConfig(folderPath);
    loadPlayerConfig(folderPath);
}

void MainMenuState::checkRequiredFiles(const std::string& folderPath)
{
    static const std::vector<std::string> Required = {"backpack.txt",
                                                      "equipment.txt",
                                                      "item.txt",
                                                      "mobloot.txt",
                                                      "quest.txt",
                                                      "shop.txt",
                                                      "skills.txt",
                                                      "stats.txt"};

    for (const auto& file : Required)
    {
        const auto path = std::filesystem::path(folderPath) / file;
        if (!std::filesystem::exists(path))
        {
            throw MissingFileException(file, "Required file not found: " + path.string());
        }
    }
    std::cout << "All required files are present in the folder." << std::endl;
}

void MainMenuState::loadPlayerConfig(const std::string& folderPath)
{
    PlayerConfigParser parser(*GetContext().GetItemManager());
    try
    {
        parser.ParseFromFile(folderPath);
        std::cout << "PlayerConfigParser done" << std::endl;
    }
    catch (const char* msg)
    {
        showError(std::string("PlayerConfigParser error: ") + (msg ? msg : "(null message)"));
        return;
    }
    catch (const std::exception& e)
    {
        showError("PlayerConfigParser error: " + std::string(e.what()));
        return;
    }
    catch (...)
    {
        showError("Unknown exception in PlayerConfigParser");
        return;
    }

    // Apply equipment/backpack
    GetContext().GetEquipment()->setEquipmentData(*GetContext().GetItemManager(),
                                                  parser.GetEquipmentData());
    GetContext().GetBackpack()->setBackpackData(parser.GetBackpackData(), 8, 4);

    // Prepare navigation and spawn player
    setupNavigationGrid();
    spawnCharacter(parser);
}

/**
 * Setup the navigation grid for the selected window size.
 */
void MainMenuState::setupNavigationGrid()
{
    sf::Vector2u                   windowSize = GetContext().GetWindow()->getSize();
    std::vector<std::vector<bool>> grid       = {
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
}

/**
 * Spawn the player's character based on the parsed configuration.
 */
void MainMenuState::spawnCharacter(const PlayerConfigParser& parser)
{
    const auto&       stats      = parser.GetUnitStats();
    const std::string charType   = stats.at("TYPE");
    NavigationGrid*   navGrid    = GetContext().GetNavigationGrid();
    sf::Vector2u      windowSize = GetContext().GetWindow()->getSize();

    // Helper to compute spawn position
    auto computePos = [&](float fraction) {
        return sf::Vector2f(windowSize.x * fraction, windowSize.y * 0.76f);
    };

    // Create & scale the correct derived type
    std::unique_ptr<Character> character;
    if (charType == "Fighter")
    {
        auto ptr = std::make_unique<Fighter>(
            charType, computePos(1.0f / 6.0f), *navGrid, false, GetContext());
        ptr->SetScale(10.f, 10.f);
        character = std::move(ptr);
    }
    else if (charType == "Mage")
    {
        auto ptr =
            std::make_unique<Mage>(charType, computePos(2.0f / 6.0f), *navGrid, false, GetContext());
        ptr->SetScale(10.f, 10.f);
        character = std::move(ptr);
    }
    else if (charType == "Assassin")
    {
        auto ptr = std::make_unique<Assassin>(
            charType, computePos(3.0f / 6.0f), *navGrid, false, GetContext());
        ptr->SetScale(10.f, 10.f);
        character = std::move(ptr);
    }
    else if (charType == "Necromancer")
    {
        auto ptr = std::make_unique<Necromancer>(
            charType, computePos(4.0f / 6.0f), *navGrid, false, GetContext());
        ptr->SetScale(10.f, 10.f);
        character = std::move(ptr);
    }
    else if (charType == "Berserker")
    {
        auto ptr = std::make_unique<Berseker>(
            charType, computePos(5.0f / 6.0f), *navGrid, false, GetContext());
        ptr->SetScale(10.f, 10.f);
        character = std::move(ptr);
    }
    else
    {
        throw InvalidCharacterTypeException(charType);
    }

    // Register in UnitManager
    const unsigned int id = character->GetId();
    GetContext().SetCharacterId(id);
    GetContext().GetUnitManager()->AddUnit(std::move(character));

    // Configure the base Unit
    auto* baseUnit = GetContext().GetUnitManager()->GetUnit(id);
    baseUnit->SetName(stats.at("NAME"));

    Stats tempStats;
    tempStats.strength                 = std::stoi(stats.at("STRENGTH"));
    tempStats.agility                  = std::stoi(stats.at("AGILITY"));
    tempStats.intelligence             = std::stoi(stats.at("INTELLIGENCE"));
    tempStats.buffMultiplier           = std::stof(stats.at("BUFF_MULTIPLIER"));
    tempStats.criticalStrikeMultiplier = std::stof(stats.at("CRITICAL_STRIKE_MULTIPLIER"));
    tempStats.criticalStrikeChance     = std::stof(stats.at("CRITICAL_STRIKE_CHANCE"));
    tempStats.skipTurnChance           = std::stof(stats.at("SKIPTURNCHANCE"));
    tempStats.luck                     = std::stoi(stats.at("LUCK"));
    tempStats.physicalDefense          = std::stoi(stats.at("PHYSICAL_DEFENSE"));
    tempStats.magicDefense             = std::stoi(stats.at("MAGIC_DEFENSE"));
    tempStats.dodgeChance              = std::stof(stats.at("DODGE_CHANCE"));
    tempStats.accuracy                 = std::stof(stats.at("ACCURACY"));
    tempStats.statusResistance         = std::stof(stats.at("STATUS_RESISTANCE"));
    tempStats.hasteMultiplier          = std::stof(stats.at("HASTE_MULTIPLIER"));
    tempStats.resourceCostMul          = std::stof(stats.at("RESOURCE_COST_MULTIPLIER"));

    baseUnit->SetStats(tempStats);
    baseUnit->SetActive(false);

    // Configure the Character subclass
    auto*       charUnit  = GetContext().GetUnitManager()->GetUnitOfType<Character>(id);
    const auto& charStats = parser.GetCharStats();
    charUnit->SetGold(std::stoi(charStats.at("GOLD")));
    charUnit->SetLevel(std::stoi(charStats.at("LEVEL")));
    charUnit->SetExp(std::stoi(charStats.at("EXP")));
    charUnit->SetMastery(std::stoi(charStats.at("MASTERY")));

    // Configure skills
    for (const auto& skill : parser.GetSkillTree())
    {
        const_cast<SkillTree*>(charUnit->GetSkillTree())->setActive(skill);
    }

    // testing
    auto activeSkill = charUnit->GetSkillTree()->getActiveSkill();
    for (const auto& skillPtr : activeSkill)
    {
        std::cout << "Active skill: " << skillPtr->getName() << std::endl;
    }
}

void MainMenuState::showError(const std::string& message)
{
    m_showErrorPopup = true;
    m_errorMessage   = message;
}

void MainMenuState::Pause()
{
    m_backsound.pause();
}

void MainMenuState::Resume()
{
    m_backsound.play();
}

void MainMenuState::Exit()
{
    m_backsound.stop();
}

void MainMenuState::parseNonPlayerConfig(const std::string& folderPath)
{
    // Item parsing
    {
        ItemConfigParser parser;
        try
        {
            parser.ParseFromFile(folderPath);
            std::cout << "ItemConfigParser done" << std::endl;
        }
        catch (const std::exception& e)
        {
            showError("ItemConfigParser error: " + std::string(e.what()));
            return;
        }
        catch (const char* msg)
        {
            showError(std::string("ItemConfigParser error: ") + (msg ? msg : "(null message)"));
            return;
        }
        catch (...)
        {
            showError("ItemConfigParser unknown error");
            return;
        }
        try
        {
            GetContext().GetItemManager()->setItemDatabase(parser.GetData());
            std::cout << "ItemManager set" << std::endl;
        }
        catch (const std::exception& e)
        {
            showError("ItemManager error: " + std::string(e.what()));
            return;
        }
        catch (const char* msg)
        {
            showError(std::string("ItemManager error: ") + (msg ? msg : "(null message)"));
            return;
        }
        catch (...)
        {
            showError("ItemManager unknown error");
            return;
        }
    }

    // Mob loot parsing
    {
        try
        {
            GetContext().GetMobLootConfigParser()->ParseFromFile(folderPath);
            std::cout << "MobLootConfigParser done" << std::endl;
        }
        catch (const std::exception& e)
        {
            showError("MobLootConfigParser error: " + std::string(e.what()));
            return;
        }
        catch (const char* msg)
        {
            showError(std::string("MobLootConfigParser error: ") + (msg ? msg : "(null message)"));
            return;
        }
        catch (...)
        {
            showError("MobLootConfigParser unknown error");
            return;
        }
    }

    // Quest parsing
    {
        QuestConfigParser parser;
        try
        {
            parser.ParseFromFile(folderPath);
            GetContext().GetQuestGenerator()->loadQuestData(parser.GetQuestData());
            std::cout << "QuestConfigParser done" << std::endl;
        }
        catch (const std::exception& e)
        {
            showError("QuestConfigParser error: " + std::string(e.what()));
            return;
        }
        catch (const char* msg)
        {
            showError(std::string("QuestConfigParser error: ") + (msg ? msg : "(null message)"));
            return;
        }
        catch (...)
        {
            showError("QuestConfigParser unknown error");
            return;
        }
    }

    // Shop parsing
    {
        ShopConfigParser parser;
        try
        {
            parser.ParseFromFile(folderPath);
            auto& shop = *GetContext().GetShop();
            shop.SetShopData(parser.GetData());
            shop.SetMasterItems(GetContext().GetItemManager()->getAllItems());
            shop.restock();
            std::cout << "ShopConfigParser done" << std::endl;
        }
        catch (const std::exception& e)
        {
            showError("ShopConfigParser error: " + std::string(e.what()));
            return;
        }
        catch (const char* msg)
        {
            showError(std::string("ShopConfigParser error: ") + (msg ? msg : "(null message)"));
            return;
        }
        catch (...)
        {
            showError("ShopConfigParser unknown error");
            return;
        }
    }
}