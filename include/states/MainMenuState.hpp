#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>

#include <atomic>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "core/GameContext.hpp"
#include "core/State.hpp"

#include "ui/Button.hpp"

// Include ImGuiFileDialog
#include "external/ImGuiFileDialog.h"

class MainMenuState : public State
{
public:
    MainMenuState(const GameContext& context);
    void               Init() override;
    State::StateChange HandleInput(const sf::Event& event, const sf::Window& window) override;
    State::StateChange Update(const sf::Time& dt) override;
    void               Draw(sf::RenderWindow& window) override;
    void               RenderUI() override;
    void               Exit() override;  // Important for thread cleanup!
private:
    sf::Sound m_backsound;
    sf::Sound m_buttonHoverSound;
    sf::Sound m_buttonClickSound;
    sf::Font  m_font;

    sf::Texture m_backgroundTexture;
    sf::Texture m_logoTexture;
    sf::Texture m_logoShadowTexture;
    sf::Texture m_buttonTexture;

    sf::Sprite m_backgroundSprite;
    sf::Sprite m_logoSprite;
    sf::Sprite m_logoShadowSprite;

    Button m_startButton;
    Button m_newGameButton;
    Button m_loadButton;
    Button m_exitButton;

    State::StateChange m_pendingStateChange;

    // File dialog related members
    bool        m_showFileDialog = false;
    std::string m_selectedFolder;
    bool        m_showErrorPopup = false;
    std::string m_errorMessage;
    bool        m_showExitPopup = false;

    // Required files
    const std::vector<std::string> m_requiredFiles = {
        "item.txt", "shop.txt", "backpack.txt", "mobloot.txt", "equipment.txt"};

    /**
     * @brief Validates if the selected folder contains all required files
     *
     * @param folderPath Path to the folder to check
     * @return true if all required files exist, false otherwise
     */
    bool validateFolder(const std::string& folderPath);

    /**
     * @brief Shows an error message in an ImGui popup
     *
     * @param message The error message to display
     */
    void showError(const std::string& message);
};