#include "states/MainMenuState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "exception/Exception.hpp"
#include "imgui.h"
#include "states/ChooseCharacterState.hpp"
#include "states/WorldState.hpp"

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
        m_pendingStateChange =
            StateChange {StateAction::PUSH, std::make_unique<WorldState>(GetContext())};
    });

    m_newGameButton.setOnClickCallback([this]() {
        std::cout << "New Game button clicked" << std::endl;
        // change this state to not rendered
        m_showSaveNamePopup = true;
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
    if (m_showSaveNamePopup)
    {
        ImGui::OpenPopup("Enter Save Name");
        m_showSaveNamePopup = false;
    }
    if (ImGui::BeginPopupModal("Enter Save Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Please enter a name for your save folder:");
        ImGui::InputText("##SaveName", m_saveNameBuf, sizeof(m_saveNameBuf));
        ImGui::Separator();
        float w = 80.0f;
        if (ImGui::Button("OK", ImVec2(w, 0)))
        {
            // build full path under data/
            std::filesystem::path savePath = std::filesystem::path("data") / m_saveNameBuf;
            if (std::filesystem::exists(savePath))
            {
                // folder already exists → error popup
                showError("Save folder already exists: " + savePath.string());
            }
            else
            {
                try
                {
                    std::filesystem::create_directories(savePath);
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
                                                   savePath / fname,
                                                   std::filesystem::copy_options::overwrite_existing);
                    }
                    catch (const std::exception& e)
                    {
                        showError("Failed to copy " + std::string(fname) + ": " + e.what());
                        break;
                    }
                }
                // OK to use this name
                GetContext().SetCurrentFolderName(m_saveNameBuf);
                ImGui::CloseCurrentPopup();
                parseNonPlayerConfig(savePath.string());
                m_pendingStateChange = StateChange {
                    StateAction::PUSH, std::make_unique<ChooseCharacterState>(GetContext())};
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(w, 0)))
        {
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

                try
                {
                    // ← SAVE the loaded folder name into GameContext:
                    GetContext().SetCurrentFolderName(m_selectedFolder);
                    validateFolder(m_selectedFolder);
                    m_startButton.setActive(true);
                    std::cout << "Selected valid save folder: " << m_selectedFolder << std::endl;
                }
                catch (const FileNotFoundException& e)
                {
                    std::string message = "File not found: " + e.getFilename();
                    if (!e.getErrorMessage().empty())
                    {
                        message += " - " + e.getErrorMessage();
                    }
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const LineTooShortException& e)
                {
                    std::string message = "Line too short in file: " + e.getFilename();
                    if (!e.getErrorMessage().empty())
                    {
                        message += " - " + e.getErrorMessage();
                    }
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const InvalidFormatException& e)
                {
                    std::string message = "Invalid format in file: " + e.getFilename();
                    if (!e.getErrorMessage().empty())
                    {
                        message += " - " + e.getErrorMessage();
                    }
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const ResourceNotFoundException& e)
                {
                    std::string message = "Resource not found: " + e.getFilename();
                    if (!e.getErrorMessage().empty())
                    {
                        message += " - " + e.getErrorMessage();
                    }
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const MissingFileException& e)
                {
                    std::string message = "Missing file: " + e.getFilename();
                    if (!e.getErrorMessage().empty())
                    {
                        message += " - " + e.getErrorMessage();
                    }
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const InvalidCharacterTypeException& e)
                {
                    // Create a new string with the message to ensure proper lifetime
                    std::string message = std::string("Invalid character type: ") + e.what();
                    std::cout << message << std::endl;
                    showError(message);
                }
                catch (const std::exception& e)
                {
                    // Create a new string with the message to ensure proper lifetime
                    std::string message = std::string("An error occurred: ") + e.what();
                    std::cout << message << std::endl;
                    showError(message);
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

void MainMenuState::validateFolder(const std::string& folderPath)
{
    m_requiredFiles = {"backpack.txt",
                       "equipment.txt",
                       "item.txt",
                       "mobloot.txt",
                       "quest.txt",
                       "shop.txt",
                       "skills.txt",
                       "stats.txt"};

    try
    {
        // Check if all required files exist in the folder
        for (const auto& requiredFile : m_requiredFiles)
        {
            std::filesystem::path filePath = std::filesystem::path(folderPath) / requiredFile;

            if (!std::filesystem::exists(filePath))
            {
                throw MissingFileException(requiredFile,
                                           "Required file not found: " + filePath.string());
            }
        }

        std::cout << "All required files are present in the folder." << std::endl;

        // Parse the files
        parseNonPlayerConfig(folderPath);

        PlayerConfigParser playerConfigParser(*GetContext().GetItemManager());
        try
        {
            playerConfigParser.ParseFromFile(folderPath);
            std::cout << "PlayerConfigParser done" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::string error = "PlayerConfigParser error: " + std::string(e.what());
            std::cout << error << std::endl;
            showError(error);
            return;
        }
        catch (const char* msg)
        {
            std::string error = "PlayerConfigParser error: ";
            if (msg)
                error += msg;
            else
                error += "(null message)";
            std::cout << error << std::endl;
            showError(error);
            return;
        }
        catch (...)
        {
            std::string error = "Unknown exception in PlayerConfigParser";
            std::cout << error << std::endl;
            showError(error);
            return;
        }

        GetContext().GetEquipment()->setEquipmentData(*GetContext().GetItemManager(),
                                                      playerConfigParser.GetEquipmentData());

        GetContext().GetBackpack()->setBackpackData(playerConfigParser.GetBackpackData(), 8, 4);

        // std::cout << "Character Stats:" << std::endl;
        // for (const auto& stat : playerConfigParser.GetCharStats())
        // {
        //     std::cout << stat.first << ": " << stat.second << std::endl;
        // }

        // std::cout << "Unit Stats:" << std::endl;
        // for (const auto& stat : playerConfigParser.GetUnitStats())
        // {
        //     std::cout << stat.first << ": " << stat.second << std::endl;
        // }

        // std::cout << "Type Stats:" << std::endl;
        // for (const auto& stat : playerConfigParser.GetTypeStats())
        // {
        //     std::cout << stat.first << ": " << stat.second << std::endl;
        // }

        // std::cout << "Skill Tree:" << std::endl;
        // for (const auto& skill : playerConfigParser.GetSkillTree())
        // {
        //     std::cout << skill << std::endl;
        // }

        std::cout << "PlayerConfigParser done" << std::endl;

        // Rest of the code...
        std::string charType = playerConfigParser.GetTypeStats().at("TYPE");

        std::cout << "Character type: " << charType << std::endl;

        sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

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

        if (charType == "Fighter")
        {
            auto fighter = std::make_unique<Fighter>("Fighter",
                                                     sf::Vector2f(windowSize.x * (1.0f / 6),
                                                                  windowSize.y * 0.76f),
                                                     *navGrid,
                                                     false,
                                                     GetContext());

            fighter->SetScale(10.f, 10.f);

            GetContext().SetCharacterId(fighter->GetId());

            GetContext().GetUnitManager()->AddUnit(std::move(fighter));
        }
        else if (charType == "Mage")
        {
            auto mage =
                std::make_unique<Mage>("Mage",
                                       sf::Vector2f(windowSize.x * (2.0f / 6), windowSize.y * 0.76f),
                                       *navGrid,
                                       false,
                                       GetContext());

            mage->SetScale(10.f, 10.f);
            GetContext().SetCharacterId(mage->GetId());
            GetContext().GetUnitManager()->AddUnit(std::move(mage));
        }
        else if (charType == "Assassin")
        {
            auto assassin = std::make_unique<Assassin>("Assassin",
                                                       sf::Vector2f(windowSize.x * (3.0f / 6),
                                                                    windowSize.y * 0.76f),
                                                       *navGrid,
                                                       false,
                                                       GetContext());

            assassin->SetScale(10.f, 10.f);
            GetContext().SetCharacterId(assassin->GetId());
            GetContext().GetUnitManager()->AddUnit(std::move(assassin));
        }
        else if (charType == "Necromancer")
        {
            auto necromancer = std::make_unique<Necromancer>("Necromancer",
                                                             sf::Vector2f(windowSize.x * (4.0f / 6),
                                                                          windowSize.y * 0.76f),
                                                             *navGrid,
                                                             false,
                                                             GetContext());

            necromancer->SetScale(10.f, 10.f);
            GetContext().SetCharacterId(necromancer->GetId());
            GetContext().GetUnitManager()->AddUnit(std::move(necromancer));
        }
        else if (charType == "Berserker")
        {
            auto berseker = std::make_unique<Berseker>("Berserker",
                                                       sf::Vector2f(windowSize.x * (5.0f / 6),
                                                                    windowSize.y * 0.76f),
                                                       *navGrid,
                                                       false,
                                                       GetContext());

            berseker->SetScale(10.f, 10.f);
            GetContext().SetCharacterId(berseker->GetId());
            GetContext().GetUnitManager()->AddUnit(std::move(berseker));
        }
        else
        {
            throw InvalidCharacterTypeException(charType);
        }

        GetContext()
            .GetUnitManager()
            ->GetUnit(GetContext().GetCharacterId())
            ->SetName(playerConfigParser.GetUnitStats().at("NAME"));

        GetContext().GetUnitManager()->GetUnit(GetContext().GetCharacterId())->SetActive(false);

        std::cout << "Unit created and added to UnitManager" << std::endl;
    }
    catch (const FileNotFoundException& e)
    {
        std::string message = "File not found: " + e.getFilename();
        if (!e.getErrorMessage().empty())
        {
            message += " - " + e.getErrorMessage();
        }
        std::cout << message << std::endl;
        showError(message);
    }
    catch (const MissingFileException& e)
    {
        std::string message = "Missing file: " + e.getFilename();
        if (!e.getErrorMessage().empty())
        {
            message += " - " + e.getErrorMessage();
        }
        std::cout << message << std::endl;
        showError(message);
    }
    // Other specific exceptions...
    catch (const std::exception& e)
    {
        std::string error = "Standard exception: " + std::string(e.what());
        std::cout << error << std::endl;
        showError(error);
    }
    catch (const char* msg)
    {
        std::string error = "String exception: ";
        if (msg)
            error += msg;
        else
            error += "(null message)";
        std::cout << error << std::endl;
        showError(error);
    }
    catch (...)
    {
        std::string error = "Unknown exception type caught";
        std::cout << error << std::endl;
        showError(error);
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
    // Wrap each major operation in its own try-catch block to isolate the issue
    ItemConfigParser itemConfigParser;
    try
    {
        itemConfigParser.ParseFromFile(folderPath);
        std::cout << "ItemConfigParser done" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::string error = "ItemConfigParser error: " + std::string(e.what());
        std::cout << error << std::endl;
        showError(error);
        return;
    }
    catch (const char* msg)
    {
        std::string error = "ItemConfigParser error: ";
        if (msg)
            error += msg;
        else
            error += "(null message)";
        std::cout << error << std::endl;
        showError(error);
        return;
    }
    catch (...)
    {
        std::string error = "Unknown exception in ItemConfigParser";
        std::cout << error << std::endl;
        showError(error);
        return;
    }

    try
    {
        GetContext().GetItemManager()->setItemDatabase(itemConfigParser.GetData());
        std::cout << "ItemManager set" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::string error = "ItemManager error: " + std::string(e.what());
        std::cout << error << std::endl;
        showError(error);
        return;
    }
    catch (const char* msg)
    {
        std::string error = "ItemManager error: ";
        if (msg)
            error += msg;
        else
            error += "(null message)";
        std::cout << error << std::endl;
        showError(error);
        return;
    }
    catch (...)
    {
        std::string error = "Unknown exception in ItemManager";
        std::cout << error << std::endl;
        showError(error);
        return;
    }

    MobLootConfigParser mobLootConfigParser;
    mobLootConfigParser.ParseFromFile(folderPath);

    std::cout << "MobLootConfigParser done" << std::endl;

    // TODO: Insert the mob loot data into the game context

    QuestConfigParser questConfigParser;
    questConfigParser.ParseFromFile(folderPath);

    std::cout << "QuestConfigParser done" << std::endl;

    // TODO: Insert the quest data into the game context

    ShopConfigParser shopConfigParser;
    shopConfigParser.ParseFromFile(folderPath);

    std::cout << "ShopConfigParser done" << std::endl;

    GetContext().GetShop()->SetShopData(shopConfigParser.GetData());
    GetContext().GetShop()->SetMasterItems(itemConfigParser.GetData());
    GetContext().GetShop()->restock();
}