#include "states/DialogState.hpp"

#include <iostream>

#include "core/ResourceManager.hpp"

#include "imgui.h"
#include "states/ShopState.hpp"

// Constructor
DialogState::DialogState(GameContext&                    context,
                         const std::vector<std::string>& dialogTextures,
                         const std::vector<std::string>& dialogDescriptions,
                         int                             dialogCount,
                         std::unique_ptr<State>          destinationState)
    : State(context),
      m_dialogTextures(dialogTextures),
      m_dialogDescriptions(dialogDescriptions),
      m_dialogCount(dialogCount),
      m_currentDialog(0),
      m_destinationState(std::move(destinationState)),
      m_currentDialogTexture(nullptr),
      m_font(GetContext().GetResourceManager()->GetFont("main_font")),
      m_pendingStateChange({StateAction::NONE}),
      m_fullText(""),
      m_displayedText(""),
      m_currentChar(0),
      m_typewriterTimer(sf::Time::Zero),
      m_typewriterDelay(sf::milliseconds(50)),  // Adjust for speed of typewriter
      m_typewriterFinished(false)
{
    SetName("Dialog State");
}

void DialogState::Init()
{
    // Start with first dialog texture
    if (!m_dialogTextures.empty() && m_currentDialog < m_dialogTextures.size())
    {
        m_currentDialogTexture =
            &GetContext().GetResourceManager()->GetTexture(m_dialogTextures[m_currentDialog]);
    }

    // Initialize typewriter for first dialog
    if (!m_dialogDescriptions.empty() && m_currentDialog < m_dialogDescriptions.size())
    {
        m_fullText           = m_dialogDescriptions[m_currentDialog];
        m_displayedText      = "";
        m_currentChar        = 0;
        m_typewriterTimer    = sf::Time::Zero;
        m_typewriterFinished = false;
    }
}

void DialogState::NextDialog()
{
    m_currentDialog++;

    // Check if we've reached the end of dialogs
    if (m_currentDialog >= m_dialogCount || m_currentDialog >= m_dialogTextures.size())
    {
        // All dialogs shown, proceed to destination state
        if (m_destinationState)
        {
            m_pendingStateChange = StateChange {StateAction::REPLACE, std::move(m_destinationState)};
        }
        else
        {
            // Default: push ShopState
            m_pendingStateChange =
                StateChange {StateAction::REPLACE, std::make_unique<ShopState>(GetContext())};
        }
    }
    else
    {
        // Load next dialog texture
        m_currentDialogTexture =
            &GetContext().GetResourceManager()->GetTexture(m_dialogTextures[m_currentDialog]);

        // Reset typewriter for next dialog
        if (m_currentDialog < m_dialogDescriptions.size())
        {
            m_fullText           = m_dialogDescriptions[m_currentDialog];
            m_displayedText      = "";
            m_currentChar        = 0;
            m_typewriterTimer    = sf::Time::Zero;
            m_typewriterFinished = false;
        }
    }
}

void DialogState::SkipTypewriter()
{
    if (!m_typewriterFinished)
    {
        // Skip to end of current dialog
        m_displayedText      = m_fullText;
        m_currentChar        = m_fullText.length();
        m_typewriterFinished = true;
    }
    else
    {
        // Move to next dialog
        NextDialog();
    }
}

State::StateChange DialogState::ProcessEvent(const sf::Event& event)
{
    // SFML3: use is<>/getIf<> instead of type/key
    if (auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        switch (key->code)
        {
            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                SkipTypewriter();
                break;

            case sf::Keyboard::Key::Escape:
                m_pendingStateChange = StateChange {StateAction::POP};
                break;

            default:
                break;
        }
    }

    // If we queued a pop, return it immediately
    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        StateChange change   = std::move(m_pendingStateChange);
        m_pendingStateChange = {StateAction::NONE};
        return change;
    }

    return StateChange {StateAction::NONE};
}

State::StateChange DialogState::Update(const sf::Time& dt)
{
    // real-time mouse input (scoped enum)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        SkipTypewriter();

    // typewriter animation
    if (!m_typewriterFinished && m_currentChar < m_fullText.size())
    {
        m_typewriterTimer += dt;
        while (m_typewriterTimer >= m_typewriterDelay && m_currentChar < m_fullText.size())
        {
            m_displayedText += m_fullText[m_currentChar++];
            m_typewriterTimer -= m_typewriterDelay;
        }
        if (m_currentChar >= m_fullText.size())
            m_typewriterFinished = true;
    }

    if (m_pendingStateChange.GetAction() != StateAction::NONE)
    {
        StateChange change   = std::move(m_pendingStateChange);
        m_pendingStateChange = {StateAction::NONE};
        return change;
    }

    return StateChange {StateAction::NONE};
}

void DialogState::Draw(sf::RenderWindow& window)
{
    // Clear the window
    window.clear();

    // Draw the dialog texture as background
    if (m_currentDialogTexture)
    {
        sf::Sprite backgroundSprite(*m_currentDialogTexture);
        backgroundSprite.setOrigin({0, 0});
        backgroundSprite.setPosition({0, 0});

        // Scale to fit window if needed
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f scale(static_cast<float>(windowSize.x) / m_currentDialogTexture->getSize().x,
                           static_cast<float>(windowSize.y) / m_currentDialogTexture->getSize().y);
        backgroundSprite.setScale(scale);

        window.draw(backgroundSprite);
    }
}

void DialogState::RenderUI()
{
    // Create overlayed dialog box for text
    sf::Vector2u windowSize = GetContext().GetWindow()->getSize();

    // Position dialog box at bottom of screen
    ImVec2 dialogSize = ImVec2(windowSize.x * 0.8f - 70.0f, 150.0f);
    ImVec2 dialogPos  = ImVec2(windowSize.x * 0.1f - 80.0f, windowSize.y - 230.0f);

    ImGui::SetNextWindowPos(dialogPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(dialogSize, ImGuiCond_Always);

    // Style the dialog window
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));

    ImGui::Begin("Dialog",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    // Display current dialog description with typewriter effect
    ImGui::SetWindowFontScale(1.2f);
    ImGui::TextWrapped("%s", m_displayedText.c_str());
    ImGui::SetWindowFontScale(1.0f);

    // Show dialog progress indicator
    ImGui::Dummy(ImVec2(0, 5));

    // Show visual indicator based on state
    if (!m_typewriterFinished)
    {
        ImGui::TextDisabled("Click to skip...");
    }
    else
    {
        ImGui::Text("Click to continue...");
    }

    // Show dialog counter
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
    ImGui::Text(
        "(%d/%d)", m_currentDialog + 1, std::min(m_dialogCount, (int)m_dialogDescriptions.size()));

    ImGui::End();

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void DialogState::Exit()
{
    // Cleanup if needed
}