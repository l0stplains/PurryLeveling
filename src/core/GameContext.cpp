#include "core/GameContext.hpp"

GameContext::GameContext()
    : m_resourceManager(nullptr),
      m_unitManager(nullptr),
      m_stateManager(nullptr),
      m_shop(nullptr),
      m_backpack(nullptr),
      m_equipment(nullptr),
      m_navigationGrid(nullptr)
{}

GameContext::~GameContext()
{
    // We don't own these pointers, so we don't delete them
}

ResourceManager* GameContext::GetResourceManager() const
{
    return m_resourceManager;
}

UnitManager* GameContext::GetUnitManager() const
{
    return m_unitManager;
}

StateManager* GameContext::GetStateManager() const
{
    return m_stateManager;
}

Backpack* GameContext::GetBackpack() const
{
    return m_backpack;
}

Equipment* GameContext::GetEquipment() const
{
    return m_equipment;
}

Shop* GameContext::GetShop() const
{
    return m_shop;
}

ItemManager* GameContext::GetItemManager() const
{
    return m_itemManager;
}

MobLootConfigParser* GameContext::GetMobLootConfigParser() const
{
    return m_mobLootConfigParser;
}

QuestGenerator* GameContext::GetQuestGenerator() const
{
    return m_questGenerator;
}

sf::RenderWindow* GameContext::GetWindow() const
{
    return m_window;
}

float GameContext::GetCurrentFPS() const
{
    return m_currentFPS;
}

unsigned int GameContext::GetFrameCounter() const
{
    return m_frameCounter;
}

sf::Time GameContext::GetFPSTimer() const
{
    return m_fpsTimer;
}

unsigned int GameContext::GetCharacterId() const
{
    return m_characterId;
}

NavigationGrid* GameContext::GetNavigationGrid() const
{
    return m_navigationGrid;
}

std::string GameContext::GetCurrentFolderName() const
{
    return m_currentFolderName;
}
void GameContext::SetResourceManager(ResourceManager* resourceManager)
{
    m_resourceManager = resourceManager;
}

void GameContext::SetUnitManager(UnitManager* unitManager)
{
    m_unitManager = unitManager;
}

void GameContext::SetStateManager(StateManager* stateManager)
{
    m_stateManager = stateManager;
}

void GameContext::SetBackpack(Backpack* backpack)
{
    m_backpack = backpack;
}

void GameContext::SetShop(Shop* shop)
{
    m_shop = shop;
}

void GameContext::SetEquipment(Equipment* equipment)
{
    m_equipment = equipment;
}

void GameContext::SetItemManager(ItemManager* itemManager)
{
    m_itemManager = itemManager;
}

void GameContext::SetMobLootConfigParser(MobLootConfigParser* mobLootConfig)
{
    m_mobLootConfigParser = mobLootConfig;
}

void GameContext::SetQuestGenerator(QuestGenerator* questGenerator)
{
    m_questGenerator = questGenerator;
}

void GameContext::SetWindow(sf::RenderWindow* window)
{
    m_window = window;
}

void GameContext::SetVSyncEnabled(bool enabled)
{
    m_vsyncEnabled = enabled;
    m_window->setVerticalSyncEnabled(m_vsyncEnabled);
    if (m_vsyncEnabled)
    {
        m_window->setFramerateLimit(0);  // Unlimited framerate
    }
    else
    {
        m_window->setFramerateLimit(60);  // Set to 60 FPS
    }
}

void GameContext::SetCharacterId(unsigned int characterId)
{
    m_characterId = characterId;
}

void GameContext::SetNavigationGrid(NavigationGrid* navGrid)
{
    m_navigationGrid = navGrid;
}

void GameContext::SetCurrentFolderName(std::string folderName)
{
    m_currentFolderName = std::move(folderName);
}

void GameContext::UpdateFPS(const sf::Time& dt)
{
    m_frameCounter++;
    m_fpsTimer += dt;
    if (m_fpsTimer.asSeconds() >= 1.0f)
    {
        m_currentFPS   = static_cast<float>(m_frameCounter) / m_fpsTimer.asSeconds();
        m_frameCounter = 0;
        m_fpsTimer     = sf::Time::Zero;
    }
}

void GameContext::ToggleVSync()
{
    m_vsyncEnabled = !m_vsyncEnabled;
    SetVSyncEnabled(m_vsyncEnabled);
}
