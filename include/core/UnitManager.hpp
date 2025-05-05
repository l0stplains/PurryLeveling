#pragma once

#include <SFML/Graphics/RenderWindow.hpp>  // Added for Draw/DrawUI
#include <SFML/System/Time.hpp>            // Added for Update
#include <SFML/Window/Keyboard.hpp>        // Added for ProcessInput

#include <algorithm>
#include <memory>
#include <string>  // Added for GetUnitByName
#include <unordered_map>
#include <vector>

// Forward declare derived types used for casting to avoid including full headers here
// (Though including them is also fine if preferred)
class Unit;
class AnimatedUnit;
class Character;

/**
 * @class UnitManager
 * @brief Manages game units, providing methods to add, remove, and render them.
 * (Description remains the same)
 */
class UnitManager
{
public:
    /**
     * @brief Default constructor for UnitManager
     */
    UnitManager() = default;  // Use default constructor

    /**
     * @brief Destructor for UnitManager
     */
    ~UnitManager() = default;  // Use default destructor

    // Prevent copying
    UnitManager(const UnitManager&)            = delete;
    UnitManager& operator=(const UnitManager&) = delete;
    // Allow moving
    UnitManager(UnitManager&&)            = default;
    UnitManager& operator=(UnitManager&&) = default;

    // --- Unit management ---
    /**
     * @brief Add a unit to the manager
     * @param unit The unit to add (takes ownership)
     */
    void AddUnit(std::unique_ptr<Unit> unit);

    /**
     * @brief Remove a unit from the manager by ID
     * @param id The ID of the unit to remove
     */
    void RemoveUnit(unsigned int id);

    /**
     * @brief Remove a unit from the manager by name
     * @param id The ID of the unit to exclude
     */
    void RemoveUnitExcept(unsigned int id);

    /**
     * @brief Get a raw pointer to a unit by its ID
     * @param id The ID of the unit to retrieve
     * @return Unit* Pointer to the unit, or nullptr if not found. Lifetime managed by UnitManager.
     */
    Unit* GetUnit(unsigned int id);

    /**
     * @brief Get a raw pointer to a unit by its name
     * @param name The name of the unit to retrieve
     * @return Unit* Pointer to the unit, or nullptr if not found. Lifetime managed by UnitManager.
     */
    Unit* GetUnitByName(const std::string& name);

    // --- Unit retrieval with type casting ---
    /**
     * @brief Get a pointer of a specific derived type by ID
     * @tparam T The derived type of the unit to retrieve (e.g., Fighter, Character)
     * @param id The ID of the unit to retrieve
     * @return T* Pointer to the unit cast to type T, or nullptr if not found or type mismatch.
     */
    template <typename T>
    T* GetUnitOfType(unsigned int id);

    /**
     * @brief Get all units of a specific derived type
     * @tparam T The derived type of the units to retrieve
     * @return std::vector<T*> Vector of raw pointers to units of the specified type.
     */
    template <typename T>
    std::vector<T*> GetAllUnitsOfType();

    // --- Unit lifecycle ---
    /**
     * @brief Update all active units in the manager
     * @param dt The time delta since the last update
     */
    void Update(const sf::Time& dt);

    /**
     * @brief Draw all active units in the manager, respecting Z-order
     * @param window The window to draw the units to
     */
    void Draw(sf::RenderWindow& window);

    /**
     * @brief Draw UI elements for relevant units (e.g., Characters)
     * @param window The window to draw the UI to
     */
    void DrawUI(sf::RenderWindow& window);  // Added DrawUI

    /**
     * @brief Process keyboard input, dispatching to controllable units (Characters)
     * @param keyCode The key code of the pressed key
     */
    void ProcessEvent(const sf::Event& event);

    // --- Z-Order Management ---
    /**
     * @brief Set the Z-order of a unit (if it's an AnimatedUnit)
     * @param id The ID of the unit
     * @param zOrder The Z-order value (higher values drawn on top)
     */
    void SetUnitZOrder(unsigned int id, int zOrder);

    /**
     * @brief Bring a unit to the front (highest Z-order) (if it's an AnimatedUnit)
     * @param id The ID of the unit
     */
    void BringUnitToFront(unsigned int id);

    // --- Utility methods ---
    /**
     * @brief Clear all units from the manager
     */
    void Clear();

    /**
     * @brief Get the total number of units managed
     * @return size_t The number of units in the manager
     */
    size_t GetUnitCount() const;

    void AddUnitImmediate(std::unique_ptr<Unit> unit);

    void RemoveUnitImmediate(unsigned int id);

private:
    std::vector<std::unique_ptr<Unit>>      m_units;        ///< Owns the Unit objects
    std::unordered_map<unsigned int, Unit*> m_unitsById;    ///< Non-owning pointers for ID lookup
    std::unordered_map<std::string, Unit*>  m_unitsByName;  ///< Non-owning pointers for Name lookup
    bool m_needsSorting = false;  ///< Flag to indicate if sorting is needed before next Draw

    // deferral buffers
    std::vector<std::unique_ptr<Unit>> m_pendingAdditions;
    std::vector<int>                   m_pendingRemovals;

    bool m_inUpdate = false;  // guard flag

    /**
     * @brief Sort units vector by Z-order (using dynamic_cast)
     */
    void SortUnitsByZOrder();
};

// Template implementations must be in the header or included file
#include "units/Unit.hpp"  // Need full Unit definition for dynamic_cast

template <typename T>
T* UnitManager::GetUnitOfType(unsigned int id)
{
    Unit* unit = GetUnit(id);  // Get base pointer
    // Attempt to cast to the requested derived type T
    return unit ? dynamic_cast<T*>(unit) : nullptr;
}

template <typename T>
std::vector<T*> UnitManager::GetAllUnitsOfType()
{
    std::vector<T*> result;
    result.reserve(m_units.size());      // Optional optimization
    for (const auto& unitPtr : m_units)  // Iterate through the vector of unique_ptr
    {
        // dynamic_cast on the raw pointer obtained from unique_ptr::get()
        if (T* derived = dynamic_cast<T*>(unitPtr.get()))
        {
            result.push_back(derived);
        }
    }
    return result;
}