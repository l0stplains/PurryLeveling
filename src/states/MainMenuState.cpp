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
    constexpr char  PopupId[] = "Enter Save Name";
    constexpr float BtnWidth  = 80.0f;

    if (m_showSaveNamePopup)
    {
        ImGui::OpenPopup(PopupId);
        m_showSaveNamePopup = false;
    }
    if (!ImGui::BeginPopupModal(PopupId, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        return;

    ImGui::Text("Please enter a name for your save folder:");
    ImGui::InputText("##SaveName", m_saveNameBuf, sizeof(m_saveNameBuf));
    ImGui::Separator();

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

    if (ImGui::Button("OK", ImVec2(BtnWidth, 0)))
        confirmSave();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(BtnWidth, 0)))
        ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
}

void MainMenuState::renderLoadSaveModal()
{
    if (!m_showFileDialog)
        return;

    ImVec2 screenSize((float)GetContext().GetWindow()->getSize().x,
                      (float)GetContext().GetWindow()->getSize().y);
    ImVec2 dialogSize(800, 600);
    ImVec2 dialogPos((screenSize.x - dialogSize.x) * 0.5f, (screenSize.y - dialogSize.y) * 0.5f);

    ImGui::SetNextWindowPos(dialogPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(dialogSize, ImGuiCond_Always);

    if (ImGuiFileDialog::Instance()->Display("ChooseFolderDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::filesystem::path absPath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::filesystem::path relPath;
            try
            {
                relPath = std::filesystem::relative(absPath, std::filesystem::current_path());
            }
            catch (...)
            {
                relPath = absPath.filename();
            }

            m_selectedFolder = relPath.string();

            try
            {
                validateFolder(m_selectedFolder);
                constexpr char dataPrefix[] = "data/";
                if (m_selectedFolder.rfind(dataPrefix, 0) == 0)
                    m_selectedFolder.erase(0, sizeof(dataPrefix) - 1);

                GetContext().SetCurrentFolderName(m_selectedFolder);
                GetContext().SetFirstSaveState(false);
                m_pendingStateChange =
                    StateChange {StateAction::PUSH, std::make_unique<WorldState>(GetContext())};
                std::cout << "Selected valid save folder: " << m_selectedFolder << std::endl;
            }
            catch (const FileNotFoundException& e)
            {
                std::string message = "File not found: " + e.getFilename();
                if (!e.getErrorMessage().empty())
                    message += " - " + e.getErrorMessage();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const LineTooShortException& e)
            {
                std::string message = "Line too short in file: " + e.getFilename();
                if (!e.getErrorMessage().empty())
                    message += " - " + e.getErrorMessage();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const InvalidFormatException& e)
            {
                std::string message = "Invalid format in file: " + e.getFilename();
                if (!e.getErrorMessage().empty())
                    message += " - " + e.getErrorMessage();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const ResourceNotFoundException& e)
            {
                std::string message = "Resource not found: " + e.getFilename();
                if (!e.getErrorMessage().empty())
                    message += " - " + e.getErrorMessage();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const MissingFileException& e)
            {
                std::string message = "Missing file: " + e.getFilename();
                if (!e.getErrorMessage().empty())
                    message += " - " + e.getErrorMessage();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const InvalidCharacterTypeException& e)
            {
                std::string message = std::string("Invalid character type: ") + e.what();
                std::cout << message << std::endl;
                showError(message);
            }
            catch (const std::exception& e)
            {
                std::string message = std::string("An error occurred: ") + e.what();
                std::cout << message << std::endl;
                showError(message);
            }
        }

        m_showFileDialog = false;
        ImGuiFileDialog::Instance()->Close();
    }
}

void MainMenuState::renderErrorModal()
{
    if (m_showErrorPopup)
    {
        ImGui::OpenPopup("Error");
        m_showErrorPopup = false;
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
}

void MainMenuState::renderExitConfirmationModal()
{
    if (m_showExitPopup)
    {
        ImGui::OpenPopup("Exit Confirmation");
        m_showExitPopup = false;
    }
    if (ImGui::BeginPopupModal("Exit Confirmation",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Are you sure you want to exit?");
        ImGui::Dummy(ImVec2(0, 4.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 4.0f));
        float buttonWidth = 60.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - 2 * buttonWidth - 10.0f);
        ImGui::SetCursorPosX(buttonPosX);

        if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
            GetContext().GetWindow()->close();
            m_pendingStateChange = StateChange {};
        }
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
    const auto&       stats      = parser.GetTypeStats();
    const std::string charType   = stats.at("TYPE");
    NavigationGrid*   navGrid    = GetContext().GetNavigationGrid();
    sf::Vector2u      windowSize = GetContext().GetWindow()->getSize();

    std::unique_ptr<Character> character;

    auto spawnAndScale = [&](auto makePtr, float fraction) {
        auto ptr = makePtr(charType,
                           sf::Vector2f(windowSize.x * fraction, windowSize.y * 0.76f),
                           *navGrid,
                           false,
                           GetContext());
        ptr->SetScale(10.f, 10.f);  // child-specific method
        character = std::move(ptr);
    };

    if (charType == "Fighter")
    {
        spawnAndScale([](auto&&... args) { return std::make_unique<Fighter>(args...); }, 1.0f / 6);
    }
    else if (charType == "Mage")
    {
        spawnAndScale([](auto&&... args) { return std::make_unique<Mage>(args...); }, 2.0f / 6);
    }
    else if (charType == "Assassin")
    {
        spawnAndScale([](auto&&... args) { return std::make_unique<Assassin>(args...); }, 3.0f / 6);
    }
    else if (charType == "Necromancer")
    {
        spawnAndScale([](auto&&... args) { return std::make_unique<Necromancer>(args...); },
                      4.0f / 6);
    }
    else if (charType == "Berserker")
    {
        spawnAndScale([](auto&&... args) { return std::make_unique<Berseker>(args...); }, 5.0f / 6);
    }
    else
    {
        throw InvalidCharacterTypeException(charType);
    }

    // Register in UnitManager
    const auto id = character->GetId();
    GetContext().SetCharacterId(id);
    GetContext().GetUnitManager()->AddUnit(std::move(character));

    // Configure the unit
    auto* baseUnit = GetContext().GetUnitManager()->GetUnit(id);
    baseUnit->SetName(parser.GetUnitStats().at("NAME"));
    baseUnit->SetActive(false);

    // Configure the character
    auto* charUnit = GetContext().GetUnitManager()->GetUnitOfType<Character>(id);
    charUnit->SetGold(std::stoi(parser.GetCharStats().at("GOLD")));
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