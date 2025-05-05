#include "units/mobs/Mob.hpp"

#include <SFML/Graphics/RectangleShape.hpp>  // Example for UI
#include <SFML/Graphics/Text.hpp>            // Example for UI

#include <iostream>  // For debug

Mob::Mob(const std::string& name)
    : Unit(name)  // Call base constructor
{
    // Mob specific stat initialization (can override AnimatedUnit defaults)
    m_maxHealth = 120;  // Example: Mobs slightly tougher
    SetHealth(m_maxHealth);
    m_maxMana = 60;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 12;
}

int Mob::GetLevel() const
{
    return m_level;
}

float Mob::GetSkillProbabability() const
{
    return m_skillProbabability;
}

void Mob::SetLevel(int level)
{
    m_level = level;
}
