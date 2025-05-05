#include "states/LoadingState.hpp"

// Constructor
LoadingState::LoadingState(GameContext& context)
    : State(context),
      m_logoTexture(GetContext().GetResourceManager()->GetTexture("main_logo")),
      m_logoSprite(m_logoTexture),
      m_resourceParser(std::make_unique<ResourceConfigParser>())
{
    // Initialize loading variables
    m_loadingComplete  = false;
    m_loadingProgress  = 0.0f;
    m_loadingError     = false;
    m_errorMessage     = "";
    m_showErrorDialog  = false;
    m_closeApplication = false;

    SetName("Loading State");
}

void LoadingState::Init()
{
    // Reset state
    m_loadingComplete           = false;
    m_loadingProgress           = 0.0f;
    m_loadingTransitionStarted  = false;
    m_loadingError              = false;
    m_showErrorDialog           = false;
    m_closeApplication          = false;
    m_errorMessage              = "";
    m_currentLoadingDescription = "Starting to load resources...";

    m_logoSprite.setOrigin({m_logoTexture.getSize().x / 2, m_logoTexture.getSize().y / 2});
    m_logoSprite.setScale({360.f / m_logoTexture.getSize().x, 360.f / m_logoTexture.getSize().y});
    m_logoSprite.setPosition({GetContext().GetWindow()->getSize().x / 2.f,
                              GetContext().GetWindow()->getSize().y / 2.f - 120.f});

    // Start the loading thread
    m_loadingThread = std::thread(&LoadingState::LoadResources, this);
}

bool LoadingState::LoadResourcesFromConfig(const std::string& configPath)
{
    try
    {
        m_resourceParser->ParseFromFile(configPath);

        const auto& resourceManager = GetContext().GetResourceManager();

        // Calculate total resources for progress tracking
        m_totalResources = 0;
        if (m_resourceParser->HasCategory("fonts"))
        {
            m_totalResources += m_resourceParser->GetResources("fonts").size();
        }
        if (m_resourceParser->HasCategory("textures"))
        {
            m_totalResources += m_resourceParser->GetResources("textures").size();
        }
        if (m_resourceParser->HasCategory("audio"))
        {
            m_totalResources += m_resourceParser->GetResources("audio").size();
        }

        if (m_totalResources == 0)
        {
            throw std::runtime_error("No resources found in configuration");
        }

        m_resourcesLoaded = 0;

        // Load fonts
        if (m_resourceParser->HasCategory("fonts"))
        {
            for (const auto& entry : m_resourceParser->GetResources("fonts"))
            {
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_currentLoadingDescription = "Loading font: " + entry.id;
                }

                try
                {
                    resourceManager->LoadFont(entry.id, entry.path);
                }
                catch (const std::exception& e)
                {
                    throw std::runtime_error("Failed to load font '" + entry.id + "' from path '" +
                                             entry.path + "': " + e.what());
                }

                m_resourcesLoaded++;
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_loadingProgress = static_cast<float>(m_resourcesLoaded) / m_totalResources;
                }
            }
        }

        // Load textures
        if (m_resourceParser->HasCategory("textures"))
        {
            for (const auto& entry : m_resourceParser->GetResources("textures"))
            {
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_currentLoadingDescription = "Loading texture: " + entry.id;
                }

                try
                {
                    resourceManager->LoadTexture(entry.id, entry.path);
                }
                catch (const std::exception& e)
                {
                    throw std::runtime_error("Failed to load texture '" + entry.id +
                                             "' from path '" + entry.path + "': " + e.what());
                }

                m_resourcesLoaded++;
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_loadingProgress = static_cast<float>(m_resourcesLoaded) / m_totalResources;
                }
            }
        }

        // Load audio
        if (m_resourceParser->HasCategory("audio"))
        {
            for (const auto& entry : m_resourceParser->GetResources("audio"))
            {
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_currentLoadingDescription = "Loading audio: " + entry.id;
                }

                try
                {
                    resourceManager->LoadSoundBuffer(entry.id, entry.path);
                }
                catch (const std::exception& e)
                {
                    throw std::runtime_error("Failed to load audio '" + entry.id + "' from path '" +
                                             entry.path + "': " + e.what());
                }

                m_resourcesLoaded++;
                {
                    std::lock_guard<std::mutex> lock(m_progressMutex);
                    m_loadingProgress = static_cast<float>(m_resourcesLoaded) / m_totalResources;
                }
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        std::lock_guard<std::mutex> lock(m_progressMutex);
        m_errorMessage    = e.what();
        m_loadingError    = true;
        m_showErrorDialog = true;
        std::cerr << "Error loading resources: " << m_errorMessage << std::endl;
        return false;
    }
}

void LoadingState::LoadResources()
{
    try
    {
        // Load resources from configuration file
        std::string configPath = "resources/config/resources.txt";
        if (!LoadResourcesFromConfig(configPath))
        {
            // Error is already handled in LoadResourcesFromConfig
            return;
        }

        // Loading complete
        {
            std::lock_guard<std::mutex> lock(m_progressMutex);
            m_loadingProgress           = 1.0f;
            m_currentLoadingDescription = "Loading complete!";
            m_loadingComplete           = true;
        }
    }
    catch (const std::exception& e)
    {
        // Handle loading errors
        std::lock_guard<std::mutex> lock(m_progressMutex);
        m_errorMessage    = e.what();
        m_loadingError    = true;
        m_showErrorDialog = true;
        std::cerr << "Error in loading thread: " << m_errorMessage << std::endl;
    }
}

State::StateChange LoadingState::ProcessEvent(const sf::Event& event)
{
    return StateChange {};
}

State::StateChange LoadingState::Update(const sf::Time& dt)
{
    // Check if we need to close the application due to error
    {
        std::lock_guard<std::mutex> lock(m_progressMutex);
        if (m_closeApplication)
        {
            // Signal to close the application
            GetContext().GetWindow()->close();
            return StateChange {};
        }
    }

    // Get the latest progress data
    {
        std::lock_guard<std::mutex> lock(m_progressMutex);

        // Check if loading has completed
        if (m_loadingComplete && !m_loadingTransitionStarted)
        {
            m_loadingTimer.restart();
            m_loadingTransitionStarted = true;
        }
    }

    // If loading is complete and the transition delay has elapsed, change state
    if (m_loadingTransitionStarted && m_loadingTimer.getElapsedTime().asSeconds() > 1.0f)
    {
        return StateChange {StateAction::REPLACE, std::make_unique<MainMenuState>(GetContext())};
    }

    return StateChange {};
}

void LoadingState::Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(0, 0, 0));

    // Draw logo
    window.draw(m_logoSprite);
}

void LoadingState::RenderUI()
{
    // Check if we need to show the error dialog
    bool        showDialog = false;
    std::string errorMsg;

    {
        std::lock_guard<std::mutex> lock(m_progressMutex);
        showDialog = m_showErrorDialog;
        errorMsg   = m_errorMessage;
    }

    if (showDialog)
    {
        ImGui::OpenPopup("ErrorPopup");
        showDialog = false;
    }

    // Calculate center position and set size just like before
    ImVec2 screenSize =
        ImVec2(GetContext().GetWindow()->getSize().x, GetContext().GetWindow()->getSize().y);
    ImVec2 dialogSize = ImVec2(400, 200);
    ImVec2 dialogPos =
        ImVec2((screenSize.x - dialogSize.x) * 0.5f, (screenSize.y - dialogSize.y) * 0.5f);

    // Set position and size before opening popup
    ImGui::SetNextWindowPos(dialogPos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(dialogSize, ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal("ErrorPopup",
                               nullptr,
                               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
    {
        // Center content vertically (optional)
        float textHeight     = ImGui::GetTextLineHeight() * 3;  // Estimated 3 lines of text
        float availableSpace = ImGui::GetContentRegionAvail().y;

        float textPosY = (availableSpace - textHeight) * 0.5f - 30.0f;

        if (textPosY > 0)
            ImGui::Dummy(ImVec2(0, textPosY));

        ImGui::TextWrapped("Error loading resources:");
        ImGui::TextWrapped("%s", errorMsg.c_str());

        if (textPosY > 0)
            ImGui::Dummy(ImVec2(0, 10.0f));

        float buttonWidth = 120.0f;
        float buttonPosX  = (ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f;
        ImGui::SetCursorPosX(buttonPosX);

        if (ImGui::Button("Exit Application", ImVec2(buttonWidth, 0)))
        {
            std::lock_guard<std::mutex> lock(m_progressMutex);
            m_closeApplication = true;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    else
    {
        // Regular loading UI when there's no error
        ImGui::Begin("Loading",
                     nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowSize(ImVec2(600, 200));
        ImGui::SetWindowPos(ImVec2(340, 480));
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetWindowCollapsed(false);
        ImGui::SetWindowFocus();

        // Create local copies of thread-shared data
        float       progress;
        int         totalResources;
        int         resourcesLoaded;
        std::string description;
        {
            std::lock_guard<std::mutex> lock(m_progressMutex);
            progress        = m_loadingProgress;
            totalResources  = this->m_totalResources;
            resourcesLoaded = this->m_resourcesLoaded;
            description     = m_currentLoadingDescription;
        }
        ImGui::Text("Loading resources (%d/%d)", resourcesLoaded, totalResources);

        // Push color styles
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.227f, 0.525f, 0.545f, 1.0f));  // Fill
                                                                                              // bar
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.109f, 0.298f, 0.325f, 1.0f));  // Background
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.082f, 0.258f, 0.298f, 1.0f));   // Border

        // Push shape styles
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);    // Rounded corners
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);  // Border thickness

        // Draw the bar
        ImGui::ProgressBar(progress);

        // Pop in reverse order
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::Text("%s", description.c_str());

        ImGui::End();
    }
}

void LoadingState::Exit()
{
    // Make sure thread has completed before leaving the state
    if (m_loadingThread.joinable())
    {
        m_loadingThread.join();
    }
}