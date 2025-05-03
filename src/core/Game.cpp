#include "core/Game.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "states/LoadingState.hpp"

Game::Game()
    : m_window(sf::VideoMode({1280, 720}), "Purry Leveling", sf::Style::None),  // Non-resizable
      m_context {},
      m_stateManager(),
      m_shop(),
      m_itemManager(),
      m_mobLootConfigParser(),
      m_questGenerator(),
      m_backpack(4, 8),
      m_equipment()
{
    // Get the desktop resolution to center the window
    sf::VideoMode desktopMode  = sf::VideoMode::getDesktopMode();
    int           windowWidth  = 1280;  // Set the window width
    int           windowHeight = 720;   // Set the window height

    m_window.setSize(sf::Vector2u(windowWidth, windowHeight));

    // Calculate the position to center the window
    int x = (desktopMode.size.x - windowWidth) / 2;
    int y = (desktopMode.size.y - windowHeight) / 2;

    // Set the window position to the calculated position (centered)
    m_window.setPosition(sf::Vector2i(x, y));

    // Set up context with pointers to resources and entities
    m_context.SetResourceManager(&m_resourceManager);
    m_context.SetUnitManager(&m_unitManager);
    m_context.SetWindow(&m_window);
    m_context.SetShop(&m_shop);
    m_context.SetBackpack(&m_backpack);
    m_context.SetEquipment(&m_equipment);
    m_context.SetItemManager(&m_itemManager);
    m_context.SetMobLootConfigParser(&m_mobLootConfigParser);
    m_context.SetQuestGenerator(&m_questGenerator);

    m_context.SetVSyncEnabled(false);

    // Initialize resource manager
    m_context.GetResourceManager()->LoadTexture("main_logo", "resources/textures/ui/main_logo.png");

    // Set icon
    sf::Image icon;
    if (icon.loadFromFile("resources/textures/ui/main_icon.png"))
    {
        m_window.setIcon(icon);
    }

    // Initialize ImGui
    m_gui.Init(m_window);

    // Start with loading state
    m_stateManager.PushState(std::make_unique<LoadingState>(m_context));
}

Game::~Game()
{
    m_gui.Shutdown();
}

void Game::Run()
{
    while (m_window.isOpen() && !m_stateManager.IsEmpty())
    {
        sf::Time dt = m_gameClock.restart();

        m_context.UpdateFPS(dt);

        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::ProcessEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        m_stateManager.ProcessEvent(*event);
        m_unitManager.ProcessEvent(*event);
        m_gui.ProcessEvent(*event, m_window);

        // Close window: exit
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void Game::Update(const sf::Time& dt)
{
    m_gui.NewFrame();
    m_stateManager.Update(dt);
    m_unitManager.Update(dt);
}

void Game::Render()
{
    m_window.clear();

    // Draw game states
    m_stateManager.Draw(m_window);
    m_unitManager.Draw(m_window);

    // Render ImGui UI
    m_stateManager.RenderUI();
    m_unitManager.DrawUI(m_window);
    m_gui.Render(m_window);

    m_window.display();
}