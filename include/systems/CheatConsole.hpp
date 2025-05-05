#pragma once

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class CheatConsole
{
public:
    using CheatCommandCallback = std::function<std::string(const std::vector<std::string>&)>;

    CheatConsole(sf::RenderWindow& window);
    ~CheatConsole();

    void registerCommand(const std::string&   command,
                         CheatCommandCallback callback,
                         const std::string&   description);

    void toggleVisibility();

    bool isVisible() const;

    void processEvent(const sf::Event& event);

    void render();

    void update(float deltaTime);

private:
    std::vector<std::string> tokenizeCommand(const std::string& command);

    void executeCommand(const std::string& commandLine);

    struct CommandInfo
    {
        CheatCommandCallback callback;
        std::string          description;
    };

    sf::RenderWindow& m_window;
    bool              m_isVisible;
    std::string       m_inputBuffer;
    std::string       m_lastFeedback;
    float             m_feedbackTimer;
    bool              m_showHelp;

    std::vector<std::string> m_commandHistory;
    int                      m_historyIndex;

    std::unordered_map<std::string, CommandInfo> m_commands;

    static constexpr size_t MAX_INPUT_CHARS       = 256;
    static constexpr float  FEEDBACK_DISPLAY_TIME = 5.0f;
    static constexpr int    MAX_HISTORY_SIZE      = 10;
};
