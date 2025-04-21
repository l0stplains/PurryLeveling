#pragma once
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "entities/Entity.hpp"

/**
 * @class EntityManager
 * @brief Manages game entities, providing methods to add, remove, and render them.
 *
 * The EntityManager class is responsible for managing game entities, including
 * adding, removing, and updating them. It provides methods to retrieve entities
 * by ID or name, as well as to get all entities of a specific type.
 */
class EntityManager
{
public:
    /**
     * @brief Default constructor for EntityManager
     */
    EntityManager();

    /**
     * @brief Destructor for EntityManager
     */
    ~EntityManager();

    // Entity management

    /**
     * @brief Add an entity to the manager
     *
     * @param entity The entity to add
     */
    void AddEntity(std::unique_ptr<Entity> entity);

    /**
     * @brief Remove an entity from the manager by ID
     *
     * @param id The ID of the entity to remove
     */
    void RemoveEntity(unsigned int id);

    /**
     * @brief Remove an entity from the manager by name
     *
     * @param name The name of the entity to remove
     */
    Entity* GetEntity(unsigned int id);

    /**
     * @brief Get an entity by its name
     *
     * @param name The name of the entity to retrieve
     * @return Entity* Pointer to the entity with the specified name, or nullptr if not found
     */
    Entity* GetEntityByName(const std::string& name);

    // Entity retrieval

    /**
     * @brief Get an entity of a specific type by ID
     *
     * @tparam T The type of the entity to retrieve
     * @param id The ID of the entity to retrieve
     * @return T* Pointer to the entity with the specified ID and type, or nullptr if not found
     */
    template <typename T>
    T* GetEntityOfType(unsigned int id);

    /**
     * @brief Get an entity of a specific type by name
     *
     * @tparam T The type of the entity to retrieve
     * @param name The name of the entity to retrieve
     * @return T* Pointer to the entity with the specified name and type, or nullptr if not found
     */
    template <typename T>
    std::vector<T*> GetAllEntitiesOfType();

    // Entity lifecycle

    /**
     * @brief Update all entities in the manager
     *
     * @param dt The time delta since the last update
     */
    void Update(const sf::Time& dt);

    /**
     * @brief Draw all entities in the manager to the specified window
     *
     * @param window The window to draw the entities to
     */
    void Draw(sf::RenderWindow& window);

    // Utility methods

    /**
     * @brief Clear all entities from the manager
     */
    void Clear();

    /**
     * @brief Get the total number of entities managed
     *
     * @return size_t The number of entities in the manager
     */
    size_t GetEntityCount() const;

private:
    std::vector<std::unique_ptr<Entity>> m_entities;  ///< Vector of unique pointers to entities
    std::unordered_map<unsigned int, Entity*> m_entitiesById;   ///< Map of entity IDs to pointers
    std::unordered_map<std::string, Entity*> m_entitiesByName;  ///< Map of entity names to pointers
};