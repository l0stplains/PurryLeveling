#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  // brings in sf::Time, sf::milliseconds, etc.
#include <SFML/Window.hpp>  // brings in Event, Keyboard, Mouse, etc.

#include <vector>

#include "states/State.hpp"

class DialogState : public State
{
public:
    DialogState(GameContext&                    context,
                const std::vector<std::string>& dialogTextures,
                const std::vector<std::string>& dialogDescriptions,
                int                             dialogCount,
                std::unique_ptr<State>          destinationState = nullptr);

    void        Init() override;
    StateChange ProcessEvent(const sf::Event& event) override;
    StateChange Update(const sf::Time& dt) override;
    void        Draw(sf::RenderWindow& window) override;
    void        RenderUI() override;
    void        Exit() override;

private:
    void NextDialog();
    void SkipTypewriter();

    std::vector<std::string> m_dialogTextures;
    std::vector<std::string> m_dialogDescriptions;
    int                      m_dialogCount;
    int                      m_currentDialog;
    std::unique_ptr<State>   m_destinationState;

    sf::Texture* m_currentDialogTexture;

    sf::Font&   m_font;
    StateChange m_pendingStateChange;

    // Typewriter animation variables
    std::string m_fullText;
    std::string m_displayedText;
    size_t      m_currentChar;
    sf::Time    m_typewriterTimer;
    sf::Time    m_typewriterDelay;
    bool        m_typewriterFinished;
};