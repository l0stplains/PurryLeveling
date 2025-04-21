#include "core/EntityManager.hpp"
EntityManager::EntityManager()
{
    // Constructor
}
EntityManager::~EntityManager()
{
    // The unique_ptr will handle cleanup automatically
}
void EntityManager::AddEntity(std::unique_ptr<Entity> entity)
{
    unsigned int id   = entity->GetId();
    std::string  name = entity->GetName();
    // Store raw pointers for fast lookup
    m_entitiesById[id] = entity.get();

    // Only store in name map if name is not empty
    if (!name.empty())
    {
        m_entitiesByName[name] = entity.get();
    }

    // Transfer ownership to the manager
    m_entities.push_back(std::move(entity));
}
void EntityManager::RemoveEntity(unsigned int id)
{
    auto it =
        std::find_if(m_entities.begin(),
                     m_entities.end(),
                     [id](const std::unique_ptr<Entity>& entity) { return entity->GetId() == id; });
    if (it != m_entities.end())
    {
        // Remove from lookup maps first
        std::string name = (*it)->GetName();
        if (!name.empty())
        {
            m_entitiesByName.erase(name);
        }

        m_entitiesById.erase(id);

        // Remove from main container
        m_entities.erase(it);
    }
}
Entity* EntityManager::GetEntity(unsigned int id)
{
    auto it = m_entitiesById.find(id);
    return (it != m_entitiesById.end()) ? it->second : nullptr;
}
Entity* EntityManager::GetEntityByName(const std::string& name)
{
    auto it = m_entitiesByName.find(name);
    return (it != m_entitiesByName.end()) ? it->second : nullptr;
}
template <typename T>
T* EntityManager::GetEntityOfType(unsigned int id)
{
    Entity* entity = GetEntity(id);
    return entity ? dynamic_cast<T*>(entity) : nullptr;
}
template <typename T>
std::vector<T*> EntityManager::GetAllEntitiesOfType()
{
    std::vector<T*> result;
    for (const auto& entity : m_entities)
    {
        if (T* derivedEntity = dynamic_cast<T*>(entity.get()))
        {
            result.push_back(derivedEntity);
        }
    }

    return result;
}
void EntityManager::Update(const sf::Time& dt)
{
    // Update all active entities
    for (auto& entity : m_entities)
    {
        if (entity->IsActive())
        {
            entity->Update(dt);
        }
    }
    // Remove any entities marked for deletion
    // This would be implemented if we had a "marked for deletion" flag
}
void EntityManager::Draw(sf::RenderWindow& window)
{
    // Draw all active entities
    for (auto& entity : m_entities)
    {
        if (entity->IsActive())
        {
            entity->Draw(window);
        }
    }
}
void EntityManager::Clear()
{
    m_entities.clear();
    m_entitiesById.clear();
    m_entitiesByName.clear();
}
size_t EntityManager::GetEntityCount() const
{
    return m_entities.size();
}