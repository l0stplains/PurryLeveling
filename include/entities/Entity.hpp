// Entity.h
#pragma once
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

class Entity
{
public:
    Entity();
    Entity(const std::string& name, const sf::Vector2f& position);
    virtual ~Entity() = default;
    // Core entity methods
    virtual void Update(const sf::Time& dt);
    virtual void Draw(sf::RenderWindow& window);
    // Position methods
    void         SetPosition(const sf::Vector2f& position);
    sf::Vector2f GetPosition() const;
    // Entity properties
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