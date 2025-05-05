#include "units/characters/Character.hpp"

#include <SFML/Graphics/RectangleShape.hpp>  // Example for UI
#include <SFML/Graphics/Text.hpp>            // Example for UI

#include <cmath>
#include <iostream>  // For debug

Character::Character(const std::string& name) : Unit(name)  // Call base constructor
{
    // Character specific stat initialization (can override AnimatedUnit defaults)
    m_maxHealth = 120;
    SetHealth(m_maxHealth);
    m_maxMana = 60;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 12;
}

int Character::GetLevel() const
{
    return m_level;
}
int Character::GetExp() const
{
    return m_exp;
}
int Character::GetGold() const
{
    return m_gold;
}
int Character::GetMastery() const
{
    return m_mastery;
}

void Character::SetLevel(int level)
{
    m_level = level;

    SetMaxHealth(80 * level / 4 * GetMaxHealthMultiplier());
    SetMaxMana(50 * GetMaxManaMultiplier());
    SetCurrentMana(GetMaxMana());
    SetHealth(GetMaxHealth());
    SetAttackDamage(15 * level / 4 * GetAttackDamageMultiplier());
}

void Character::SetExp(int exp)
{
    m_exp = exp;
    CheckLevelUp();
}
void Character::SetGold(int gold)
{
    m_gold = gold;
}
void Character::SetMastery(int mastery)
{
    m_mastery = mastery;
}
void Character::AddExp(int amount)
{
    m_exp += amount;
    CheckLevelUp();
}

void Character::AddGold(int amount)
{
    m_gold += amount;
}

void Character::AddMastery(int amount)
{
    m_mastery += amount;
}

void Character::CheckLevelUp()
{
    // Level Up Logic: Level * 100 EXP needed
    int expNeeded = std::pow(m_level, 1.7) + 100;
    while (m_active && m_exp >= expNeeded)
    {
        SetLevel(m_level + 1);
        AddMastery(1);
        m_exp -= expNeeded;

        std::cout << "Level Up! New level: " << m_level << std::endl;
        std::cout << "DMG: " << m_attackDamage << std::endl;
        std::cout << "HP: " << m_currentHealth << std::endl;
        std::cout << "Mana: " << m_currentMana << std::endl;
        std::cout << "EXP: " << m_exp << std::endl;
        std::cout << "EXP needed: " << expNeeded << std::endl;

        // NOTES: It tightly related with the exp reward calculation in chamber
        expNeeded = std::pow(m_level, 1.7) + 100;  // Calculate EXP needed for the *new* level
    }
}