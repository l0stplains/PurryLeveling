#pragma once
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "units/Unit.hpp"

/**
 * @class UnitManager
 * @brief Manages game units, providing methods to add, remove, and render them.
 *
 * The UnitManager class is responsible for managing game units, including
 * adding, removing, and updating them. It provides methods to retrieve units
 * by ID or name, as well as to get all units of a specific type.
 */
class UnitManager
{
public:
    /**
     * @brief Default constructor for UnitManager
     */
    UnitManager();

    /**
     * @brief Destructor for UnitManager
     */
    ~UnitManager();

    // Unit management

    /**
     * @brief Add a unit to the manager
     *
     * @param unit The unit to add
     */
    void AddUnit(std::unique_ptr<Unit> unit);

    /**
     * @brief Remove a unit from the manager by ID
     *
     * @param id The ID of the unit to remove
     */
    void RemoveUnit(unsigned int id);

    /**
     * @brief Remove a unit from the manager by name
     *
     * @param name The name of the unit to remove
     */
    Unit* GetUnit(unsigned int id);

    /**
     * @brief Get a unit by its name
     *
     * @param name The name of the unit to retrieve
     * @return Unit* Pointer to the unit with the specified name, or nullptr if not found
     */
    Unit* GetUnitByName(const std::string& name);

    // Unit retrieval

    /**
     * @brief Get a unit of a specific type by ID
     *
     * @tparam T The type of the unit to retrieve
     * @param id The ID of the unit to retrieve
     * @return T* Pointer to the unit with the specified ID and type, or nullptr if not found
     */
    template <typename T>
    T* GetUnitOfType(unsigned int id);

    /**
     * @brief Get a units of a specific type by name
     *
     * @tparam T The type of the unit to retrieve
     * @param name The name of the unit to retrieve
     * @return T* Pointer to the unit with the specified name and type, or nullptr if not found
     */
    template <typename T>
    std::vector<T*> GetAllUnitsOfType();

    // Unit lifecycle

    /**
     * @brief Update all units in the manager
     *
     * @param dt The time delta since the last update
     */
    void Update(const sf::Time& dt);

    /**
     * @brief Draw all units in the manager to the specified window
     *
     * @param window The window to draw the units to
     */
    void Draw(sf::RenderWindow& window);

    // Utility methods

    /**
     * @brief Clear all units from the manager
     */
    void Clear();

    /**
     * @brief Get the total number of units managed
     *
     * @return size_t The number of units in the manager
     */
    size_t GetUnitCount() const;

private:
    std::vector<std::unique_ptr<Unit>>      m_units;        ///< Vector of unique pointers to units
    std::unordered_map<unsigned int, Unit*> m_unitsById;    ///< Map of unit IDs to pointers
    std::unordered_map<std::string, Unit*>  m_unitsByName;  ///< Map of unit names to pointers
};