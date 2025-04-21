#pragma once
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

class Unit
{
public:
    Unit();
    Unit(const std::string& name, const sf::Vector2f& position);
    virtual ~Unit() = default;
    // Core entity methods
    virtual void Update(const sf::Time& dt);
    virtual void Draw(sf::RenderWindow& window);
    // Position methods
    void         SetPosition(const sf::Vector2f& position);
    sf::Vector2f GetPosition() const;
    // Unit properties
    void        SetName(const std::string& name);
    std::string GetName() const;
    void        SetActive(bool active);
    bool        IsActive() const;
    // Unique identifier
    unsigned int GetId() const;

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    std::string  m_name;
    bool         m_active;
    unsigned int m_id;

private:
    static unsigned int s_nextId;
};