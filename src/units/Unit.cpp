#include "units/Unit.hpp"

unsigned int Unit::s_nextId = 0;
Unit::Unit()
    : m_position(0.f, 0.f), m_velocity(0.f, 0.f), m_name("Unit"), m_active(true), m_id(s_nextId++)
{}
Unit::Unit(const std::string& name, const sf::Vector2f& position)
    : m_position(position), m_velocity(0.f, 0.f), m_name(name), m_active(true), m_id(s_nextId++)
{}
void Unit::Update(const sf::Time& dt)
{
    // Base implementation - move based on velocity
    m_position += m_velocity * dt.asSeconds();
}
void Unit::Draw(sf::RenderWindow& window)
{
    // Base implementation doesn't draw anything
    // Override in derived classes
}
void Unit::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
}
sf::Vector2f Unit::GetPosition() const
{
    return m_position;
}
void Unit::SetName(const std::string& name)
{
    m_name = name;
}
std::string Unit::GetName() const
{
    return m_name;
}
void Unit::SetActive(bool active)
{
    m_active = active;
}
bool Unit::IsActive() const
{
    return m_active;
}
unsigned int Unit::GetId() const
{
    return m_id;
}