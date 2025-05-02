#include "units/summons/Summon.hpp"

#include <iostream>  // For debug

Summon::Summon(const std::string& name) : Unit(name)  // Call base constructor
{
    // Summon specific stat initialization (can override AnimatedUnit defaults)
    m_maxHealth = 120;  // Example: Summons slightly tougher
    SetHealth(m_maxHealth);
    m_maxMana = 60;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 12;
}

int Summon::GetLevel() const
{
    return m_level;
}

void Summon::SetLevel(int level)
{
    m_level = level;
}