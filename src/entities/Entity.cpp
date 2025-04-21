// Entity.cpp
#include "entities/Entity.hpp"
unsigned int Entity::s_nextId = 0;
Entity::Entity()
    : m_position(0.f, 0.f), m_velocity(0.f, 0.f), m_name("Entity"), m_active(true), m_id(s_nextId++)
{}
Entity::Entity(const std::string& name, const sf::Vector2f& position)
    : m_position(position), m_velocity(0.f, 0.f), m_name(name), m_active(true), m_id(s_nextId++)
{}
void Entity::Update(const sf::Time& dt)
{
    // Base implementation - move based on velocity
    m_position += m_velocity * dt.asSeconds();
}
void Entity::Draw(sf::RenderWindow& window)
{
    // Base implementation doesn't draw anything
    // Override in derived classes
}
void Entity::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
}
sf::Vector2f Entity::GetPosition() const
{
    return m_position;
}
void Entity::SetName(const std::string& name)
{
    m_name = name;
}
std::string Entity::GetName() const
{
    return m_name;
}
void Entity::SetActive(bool active)
{
    m_active = active;
}
bool Entity::IsActive() const
{
    return m_active;
}
unsigned int Entity::GetId() const
{
    return m_id;
}