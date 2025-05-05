#include "systems/CheatConsole.hpp"

#include <algorithm>
#include <cstring>  // For std::strncpy
#include <sstream>

CheatConsole::CheatConsole(sf::RenderWindow& window)
    : m_window(window),
      m_isVisible(false),
      m_inputBuffer(""),
      m_lastFeedback(""),
      m_feedbackTimer(0.0f),
      m_showHelp(false),
      m_historyIndex(-1)
{
    // Initialize input buffer with zeros
    m_inputBuffer.reserve(MAX_INPUT_CHARS);

    // Register built-in commands
    registerCommand(
        "help",
        [this](const std::vector<std::string>& args) -> std::string {
            m_showHelp = !m_showHelp;
            return m_showHelp ? "Showing help" : "Help hidden";
        },
        "Toggle help display");

    registerCommand(
        "clear",
        [this](const std::vector<std::string>& args) -> std::string {
            m_commandHistory.clear();
            return "Command history cleared";
        },
        "Clear command history");
}

CheatConsole::~CheatConsole()
{
    // Nothing to clean up in this implementation
}

void CheatConsole::registerCommand(const std::string&   command,
                                   CheatCommandCallback callback,
                                   const std::string&   description)
{
    CommandInfo info;
    info.callback       = callback;
    info.description    = description;
    m_commands[command] = info;
}

void CheatConsole::toggleVisibility()
{
    m_isVisible = !m_isVisible;
}

bool CheatConsole::isVisible() const
{
    return m_isVisible;
}

void CheatConsole::processEvent(const sf::Event& event)
{
    // Toggle console with ~ key (or your preferred key)
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Grave)
            toggleVisibility();
    }

    // Don't process other events if console is not visible
    if (!m_isVisible)
        return;

    // Handle keyboard navigation for command history
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>(); keyPressed != nullptr)
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
        {
            // Navigate up in history
            if (!m_commandHistory.empty() &&
                m_historyIndex < static_cast<int>(m_commandHistory.size() - 1))
            {
                m_historyIndex++;
                m_inputBuffer = m_commandHistory[m_commandHistory.size() - 1 - m_historyIndex];
            }
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
        {
            // Navigate down in history
            if (m_historyIndex > 0)
            {
                m_historyIndex--;
                m_inputBuffer = m_commandHistory[m_commandHistory.size() - 1 - m_historyIndex];
            }
            else if (m_historyIndex == 0)
            {
                m_historyIndex = -1;
                m_inputBuffer  = "";
            }
        }
    }
}

void CheatConsole::update(float deltaTime)
{
    if (!m_isVisible)
        return;

    // Update feedback display timer
    if (m_feedbackTimer > 0.0f)
    {
        m_feedbackTimer -= deltaTime;
    }
}

void CheatConsole::render()
{
    if (!m_isVisible)
        return;

    // Create a transparent window for the console
    ImGui::SetNextWindowSize(ImVec2(m_window.getSize().x * 0.8f, m_window.getSize().y * 0.3f),
                             ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(m_window.getSize().x * 0.1f, 20), ImGuiCond_FirstUseEver);

    // Begin the console window
    if (ImGui::Begin("Cheat Console", &m_isVisible, ImGuiWindowFlags_NoCollapse))
    {
        // Display command history
        if (ImGui::BeginChild(
                "ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            // Display each command in history
            for (const auto& cmd : m_commandHistory)
            {
                ImGui::TextUnformatted(("$ " + cmd).c_str());
            }

            // Auto-scroll to the bottom
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        // Display feedback if timer is active
        if (m_feedbackTimer > 0.0f)
        {
            ImGui::Text("Result: %s", m_lastFeedback.c_str());
        }

        // Input text field with enter to submit
        ImGui::PushItemWidth(ImGui::GetWindowWidth() - 120);

        // Focus on the text input when the console is opened
        if (ImGui::IsWindowAppearing())
        {
            ImGui::SetKeyboardFocusHere();
        }

        static char inputBuffer[MAX_INPUT_CHARS] = "";  // Temporary char array for input
        std::strncpy(inputBuffer, m_inputBuffer.c_str(), MAX_INPUT_CHARS - 1);  // Sync with
                                                                                // m_inputBuffer
        inputBuffer[MAX_INPUT_CHARS - 1] = '\0';  // Ensure null-termination

        if (ImGui::InputText(
                "##Input", inputBuffer, MAX_INPUT_CHARS, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_inputBuffer = inputBuffer;  // Sync back to m_inputBuffer
            if (!m_inputBuffer.empty())
            {
                // Execute the command
                executeCommand(m_inputBuffer);

                // Add to history and reset
                m_commandHistory.push_back(m_inputBuffer);

                // Limit history size
                if (m_commandHistory.size() > MAX_HISTORY_SIZE)
                {
                    m_commandHistory.erase(m_commandHistory.begin());
                }

                m_inputBuffer.clear();
                m_historyIndex = -1;
                ImGui::SetKeyboardFocusHere();
            }
        }

        ImGui::PopItemWidth();

        // Execute button
        ImGui::SameLine();
        if (ImGui::Button("Execute"))
        {
            if (!m_inputBuffer.empty())
            {
                // Execute the command
                executeCommand(m_inputBuffer);

                // Add to history and reset
                m_commandHistory.push_back(m_inputBuffer);

                // Limit history size
                if (m_commandHistory.size() > MAX_HISTORY_SIZE)
                {
                    m_commandHistory.erase(m_commandHistory.begin());
                }

                m_inputBuffer.clear();
                m_historyIndex = -1;
                ImGui::SetKeyboardFocusHere();
            }
        }

        // Display help if requested
        if (m_showHelp)
        {
            if (ImGui::BeginTable("CommandsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("Command");
                ImGui::TableSetupColumn("Description");
                ImGui::TableHeadersRow();

                for (const auto& cmd : m_commands)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(cmd.first.c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(cmd.second.description.c_str());
                }

                ImGui::EndTable();
            }
        }
    }
    ImGui::End();
}

std::vector<std::string> CheatConsole::tokenizeCommand(const std::string& command)
{
    std::vector<std::string> tokens;
    std::istringstream       iss(command);
    std::string              token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void CheatConsole::executeCommand(const std::string& commandLine)
{
    std::vector<std::string> tokens = tokenizeCommand(commandLine);

    if (tokens.empty())
    {
        m_lastFeedback  = "Empty command";
        m_feedbackTimer = FEEDBACK_DISPLAY_TIME;
        return;
    }

    std::string cmdName = tokens[0];

    // Remove the command name from the tokens to get just the arguments
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    // Find and execute the command
    auto it = m_commands.find(cmdName);
    if (it != m_commands.end())
    {
        // Execute the command and get feedback
        m_lastFeedback  = it->second.callback(args);
        m_feedbackTimer = FEEDBACK_DISPLAY_TIME;
    }
    else
    {
        m_lastFeedback  = "Unknown command: " + cmdName;
        m_feedbackTimer = FEEDBACK_DISPLAY_TIME;
    }
}