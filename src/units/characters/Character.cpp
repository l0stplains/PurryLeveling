#include "units/characters/Character.hpp"

#include <SFML/Graphics/RectangleShape.hpp>  // Example for UI
#include <SFML/Graphics/Text.hpp>            // Example for UI

#include <iostream>  // For debug

Character::Character(const std::string& name, const sf::Vector2f& position)
    : Unit(name)  // Call base constructor
{
    // Character specific stat initialization (can override AnimatedUnit defaults)
    m_maxHealth = 120;  // Example: Characters slightly tougher
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
void Character::AddExp(int amount)
{
    if (!m_active)
        return;
    m_exp += amount;
    std::cout << GetName() << " gained " << amount << " EXP. Total: " << m_exp << std::endl;
    CheckLevelUp();
}

void Character::AddGold(int amount)
{
    if (!m_active)
        return;
    m_gold += amount;
    std::cout << GetName() << " gained " << amount << " Gold. Total: " << m_gold << std::endl;
}

void Character::AddMastery(int amount)
{
    if (!m_active)
        return;
    m_mastery += amount;
    std::cout << GetName() << " gained " << amount << " Mastery. Total: " << m_mastery << std::endl;
}

void Character::CheckLevelUp()
{
    // Example Level Up Logic: Level * 100 EXP needed
    int expNeeded = m_level * 100;
    while (m_active && m_exp >= expNeeded)
    {
        m_level++;
        m_exp -= expNeeded;

        // Stat increases on level up
        int healthGain = 10;
        int manaGain   = 5;
        int damageGain = 2;

        m_maxHealth += healthGain;
        SetHealth(m_maxHealth);  // Heal to full on level up
        m_maxMana += manaGain;
        SetCurrentMana(m_maxMana);  // Restore mana
        m_attackDamage += damageGain;

        std::cout << GetName() << " leveled up to Level " << m_level << "!" << std::endl;
        std::cout << "  Max HP: " << m_maxHealth << " (+ " << healthGain << ")" << std::endl;
        std::cout << "  Max MP: " << m_maxMana << " (+ " << manaGain << ")" << std::endl;
        std::cout << "  Attack: " << m_attackDamage << " (+ " << damageGain << ")" << std::endl;

        // Play level up effect/sound?

        expNeeded = m_level * 100;  // Calculate EXP needed for the *new* level
    }
}