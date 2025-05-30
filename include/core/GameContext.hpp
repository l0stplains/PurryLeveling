// GameContext.h
#pragma once
#include <SFML/Graphics.hpp>

#include "core/ResourceManager.hpp"
#include "core/UnitManager.hpp"

#include "inventory/Backpack.hpp"
#include "inventory/Equipment.hpp"
#include "items/ItemManager.hpp"
#include "parser/MobLootConfigParser.hpp"
#include "quest/QuestGenerator.hpp"
#include "shop/Shop.hpp"
#include "units/NavigationGrid.hpp"

class StateManager;  // forward declaration
// class UnitManager;  // forward declaration

/**
 * @class GameContext
 * @brief Context class for sharing resources between states
 *
 * This class provides a way to share resources such as the resource manager,
 * unit manager, and window between different game states. It also tracks of all necessary
 * information.
 *
 */
class GameContext
{
public:
    /**
     * @brief Default constructor for GameContext
     */
    GameContext();

    /**
     * @brief Destructor for GameContext
     */
    ~GameContext();

    // Getters

    /**
     * @brief Get the resource manager
     *
     * @return ResourceManager* Pointer to the resource manager
     */
    ResourceManager* GetResourceManager() const;

    /**
     * @brief Get the unit manager
     *
     * @return UnitManager* Pointer to the Unit manager
     */
    UnitManager* GetUnitManager() const;

    /**
     * @brief Get the state manager
     *
     * @return StateManager* Pointer to the state manager
     */
    StateManager* GetStateManager() const;

    /**
     * @brief Get the backpack
     *
     * @return Backpack* Pointer to the Backpack
     */
    Backpack* GetBackpack() const;

    /**
     * @brief Get the equipment
     *
     * @return Equipment* Pointer to the Equipment
     */
    Equipment* GetEquipment() const;

    /**
     * @brief Get the shop
     *
     * @return Shop* Pointer to the shop
     */
    Shop* GetShop() const;

    /**
     * @brief Get the item manager
     *
     * @return ItemManager* Pointer to the item manager
     */
    ItemManager* GetItemManager() const;

    /**
     * @brief Get the mob loot config parser
     *
     * @return MobLootConfigParser* Pointer to the mob loot config parser
     */
    MobLootConfigParser* GetMobLootConfigParser() const;

    QuestGenerator* GetQuestGenerator() const;

    /**
     * @brief Get the window
     *
     * @return sf::RenderWindow* Pointer to the SFML render window
     */
    sf::RenderWindow* GetWindow() const;

    /**
     * @brief Get the current frames per second (FPS)
     *
     * @return float Current FPS
     */
    float GetCurrentFPS() const;

    /**
     * @brief Get the frame counter
     *
     * @return unsigned int Current frame counter
     */
    unsigned int GetFrameCounter() const;

    /**
     * @brief Get the FPS timer
     *
     * @return sf::Time Current FPS timer
     */
    sf::Time GetFPSTimer() const;

    unsigned int GetCharacterId() const;

    NavigationGrid* GetNavigationGrid() const;

    std::string GetCurrentFolderName() const;

    bool GetFirstSaveState();

    // Setters

    /**
     * @brief Set the resource manager
     *
     * @param resourceManager Pointer to the resource manager
     */
    void SetResourceManager(ResourceManager* resourceManager);

    /**
     * @brief Set the unit manager
     *
     * @param unitManager Pointer to the unit manager
     */
    void SetUnitManager(UnitManager* unitManager);

    /**
     * @brief Set the state manager
     *
     * @param stateManager Pointer to the state manager
     */
    void SetStateManager(StateManager* stateManager);

    /**
     * @brief Set the backpack
     *
     * @param backpack Pointer to the backpack
     */
    void SetBackpack(Backpack* backpack);

    /**
     * @brief Set the equipment
     *
     * @param equipment to the backpack
     */
    void SetEquipment(Equipment* equipment);

    /**
     * @brief Set the shop
     *
     * @param shop Pointer to the shop
     */
    void SetShop(Shop* shop);

    /**
     * @brief Set the item manager
     *
     * @param itemManager Pointer to the item manager
     */
    void SetItemManager(ItemManager* itemManager);

    /**
     * @brief Set the mob loot config parser
     *
     * @param mobLootConfigParser Pointer to the mob loot config parser
     */
    void SetMobLootConfigParser(MobLootConfigParser* mobLootConfigParser);

    void SetQuestGenerator(QuestGenerator* questGenerator);

    /**
     * @brief Set the window
     *
     * @param window Pointer to the SFML render window
     */
    void SetWindow(sf::RenderWindow* window);

    /**
     * @brief Set the vertical sync (VSync) enabled state
     *
     * @param enabled true to enable VSync, false to disable
     */
    void SetVSyncEnabled(bool enabled);

    void SetCharacterId(unsigned int characterId);

    void SetNavigationGrid(NavigationGrid* navGrid);

    void SetCurrentFolderName(std::string folderName);

    void SetFirstSaveState(bool firstSave);

    // FPS tracking

    /**
     * @brief Update the FPS tracking
     *
     * @param dt Time elapsed since the last frame
     */
    void UpdateFPS(const sf::Time& dt);

    /**
     * @brief Toggle VSync on or off
     */
    void ToggleVSync();

private:
    // IMPORTANT NOTE: Clear everything related to the user loaded thing on WorldState::Exit();

    ResourceManager* m_resourceManager;  ///< Pointer to the resource manager
    UnitManager*     m_unitManager;      ///< Pointer to the unit manager
    StateManager*    m_stateManager;     ///< Pointer to the state manager
    Shop*            m_shop;             ///< Pointer to the shop
    ItemManager*     m_itemManager;

    MobLootConfigParser* m_mobLootConfigParser;

    std::string     m_currentFolderName;  ///< Folder name for saving/loading
    QuestGenerator* m_questGenerator;

    Backpack*  m_backpack;
    Equipment* m_equipment;
    bool       m_firstSave = true;

    sf::RenderWindow* m_window;          ///< Pointer to the SFML render window
    NavigationGrid*   m_navigationGrid;  ///< Navigation grid for movement restrictions

    // FPS tracking
    unsigned int m_frameCounter = 0;  ///< Frame counter
    sf::Time     m_fpsTimer;          ///< Timer for FPS calculation
    float        m_currentFPS = 0.f;  ///< Current FPS

    bool m_vsyncEnabled = false;  ///< Flag for VSync enabled state

    unsigned int m_characterId;  ///< selected character ID

    // more later, blum implement jir
};