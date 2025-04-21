#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "core/State.hpp"

#include "parser/ResourceConfigParser.hpp"

class LoadingState : public State
{
public:
    explicit LoadingState(GameContext& context);
    void        Init() override;
    StateChange HandleInput(const sf::Event& event, const sf::Window& window) override;
    StateChange Update(const sf::Time& dt) override;
    void        Draw(sf::RenderWindow& window) override;
    void        RenderUI() override;
    void        Exit() override;

private:
    void LoadResources();
    bool LoadResourcesFromConfig(const std::string& configPath);

    // Resource configuration parser
    std::unique_ptr<ResourceConfigParser> m_resourceParser;

    // Loading thread resources
    std::thread m_loadingThread;
    std::mutex  m_progressMutex;
    int         m_totalResources  = 0;
    int         m_resourcesLoaded = 0;
    float       m_loadingProgress;
    std::string m_currentLoadingDescription;
    bool        m_loadingComplete;
    bool        m_loadingTransitionStarted = false;
    bool        m_loadingError             = false;
    bool        m_showErrorDialog          = false;
    bool        m_closeApplication         = false;
    std::string m_errorMessage;
    sf::Clock   m_loadingTimer;

    // Visual elements
    sf::Texture& m_logoTexture;
    sf::Sprite   m_logoSprite;
};