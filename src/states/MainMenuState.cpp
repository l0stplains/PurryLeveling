#include "states/MainMenuState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "states/ChooseCharacterState.hpp"

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
      m_startButton(m_buttonTexture,
                    {90.f + m_buttonTexture.getSize().x / 2.f,
                     200.f + m_buttonTexture.getSize().y / 2.f}),
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
    m_startButton.setText("Continue", m_font, 32);
    m_newGameButton.setText("New Game", m_font, 32);
    m_loadButton.setText("Load Game", m_font, 32);
    m_exitButton.setText("Exit", m_font, 32);

    // Set button sounds
    m_startButton.setHoverSound(m_buttonHoverSound);
    m_startButton.setActive(false);  // Initially inactive
    m_startButton.setClickSound(m_buttonClickSound);
    m_newGameButton.setHoverSound(m_buttonHoverSound);
    m_newGameButton.setClickSound(m_buttonClickSound);
    m_loadButton.setHoverSound(m_buttonHoverSound);
    m_loadButton.setClickSound(m_buttonClickSound);
    m_exitButton.setHoverSound(m_buttonHoverSound);
    m_exitButton.setClickSound(m_buttonClickSound);

    // Set button callbacks
    m_startButton.setOnClickCallback([this]() {
        std::cout << "Continue button clicked" << std::endl;
        // m_pendingStateChange = StateChange{StateAction::PUSH, nullptr};
    });

    m_newGameButton.setOnClickCallback([this]() {
        std::cout << "New Game button clicked" << std::endl;
        // change this state to not rendered

        m_pendingStateChange =
            StateChange {StateAction::PUSH, std::make_unique<ChooseCharacterState>(GetContext())};
        // m_pendingStateChange = StateChange{StateAction::PUSH, nullptr};
    });

    m_loadButton.setOnClickCallback([this]() {
        m_showFileDialog = true;
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
    });

    m_exitButton.setOnClickCallback([this]() { m_showExitPopup = true; });

    // Disable continue button if no save exists (example)
    bool hasSaveGame = false;  // This would be your save detection logic
    m_startButton.setActive(hasSaveGame);
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
    if (ImGuiFileDialog::Instance()->IsOpened("ChooseFolderDlgKey"))
    {
        return StateChange {StateAction::NONE};
    }

    sf::RenderWindow* window = GetContext().GetWindow();

    // Update all buttons
    if (!m_showFileDialog && !m_showErrorPopup && !m_showExitPopup)
    {
        m_startButton.update(*window);
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
    m_startButton.draw(window);
    m_newGameButton.draw(window);
    m_loadButton.draw(window);
    m_exitButton.draw(window);
}

void MainMenuState::RenderUI()
{
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
                    // Valid folder - enable the start button
                    m_startButton.setActive(true);
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
            GetContext().GetWindow()->close();
            m_pendingStateChange = StateChange {};
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

bool MainMenuState::validateFolder(const std::string& folderPath)
{
    // Check if all required files exist in the folder
    for (const auto& requiredFile : m_requiredFiles)
    {
        std::filesystem::path filePath = std::filesystem::path(folderPath) / requiredFile;

        if (!std::filesystem::exists(filePath))
        {
            std::cout << "Missing required file: " << requiredFile << std::endl;
            return false;
        }
    }

    // TODO: Check if the files are valid (e.g., correct format, not corrupted)
    // TODO: Load the files to GameContext

    return true;
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