#include "units/Unit.hpp"

#include <iostream>  // For debug/logging if needed

// Initialize static ID counter
unsigned int Unit::s_nextId = 0;

Unit::Unit(const std::string& name) : m_id(s_nextId++), m_name(name), m_active(true)
{
    // Default stats are initialized using member initialization list in the header now
}

unsigned int Unit::GetId() const
{
    return m_id;
}
const std::string& Unit::GetName() const
{
    return m_name;
}
bool Unit::IsActive() const
{
    return m_active;
}

void Unit::RestoreMana(int amount)
{
    m_currentMana += amount;
    m_currentMana = std::min(m_currentMana, m_maxMana);  // Clamp to max mana
}

int Unit::GetHealth() const
{
    return m_currentHealth;
}
int Unit::GetMaxHealth() const
{
    return m_maxHealth;
}
int Unit::GetCurrentMana() const
{
    return m_currentMana;
}
int Unit::GetMaxMana() const
{
    return m_maxMana;
}
int Unit::GetAttackDamage() const
{
    return m_attackDamage;
}
int Unit::GetHealthRegen() const
{
    return m_healthRegen;
}
int Unit::GetManaRegen() const
{
    return m_manaRegen;
}

void Unit::SetName(const std::string& name)
{
    m_name = name;
}

void Unit::SetActive(bool active)
{
    m_active = active;
}

void Unit::SetMaxHealth(int maxHealth)
{
    m_maxHealth = std::max(0, maxHealth);
    // Ensure current health doesn't exceed new max
    m_currentHealth = std::min(m_currentHealth, m_maxHealth);
}

void Unit::SetHealth(int health)
{
    m_currentHealth = std::max(0, std::min(m_maxHealth, health));
    if (m_currentHealth == 0)
    {
        // Potentially set m_active = false here? Or leave it to TakeDamage?
        // Let's keep it simple for now, TakeDamage handles becoming inactive.
    }
}

void Unit::SetMaxMana(int maxMana)
{
    m_maxMana     = std::max(0, maxMana);
    m_currentMana = std::min(m_currentMana, m_maxMana);
}

void Unit::SetCurrentMana(int currentMana)
{
    m_currentMana = std::max(0, std::min(m_maxMana, currentMana));
}

void Unit::SetAttackDamage(int attackDamage)
{
    m_attackDamage = std::max(0, attackDamage);
}
void Unit::SetHealthRegen(int healthRegen)
{
    m_healthRegen = healthRegen;
}
void Unit::SetManaRegen(int manaRegen)
{
    m_manaRegen = manaRegen;
}

// --- Base implementations for actions affecting stats ---

void Unit::Attack(Unit& target, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();  // Already inactive, just run callback
        return;
    }

    // Basic attack logic: reduce target's health by this unit's attack damage
    target.TakeDamage(m_attackDamage, callback);
}

void Unit::TakeDamage(int damage, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();  // Already inactive, just run callback
        return;
    }

    m_currentHealth -= damage;
    // std::cout << m_name << " (Base) took " << damage << " damage. Health: " << m_health << "/" <<
    // m_maxHealth << std::endl; // Debug

    if (m_currentHealth <= 0)
    {
        m_currentHealth = 0;
        // std::cout << m_name << " (Base) became inactive." << std::endl; // Debug
    }

    // Run the callback immediately as base Unit has no animation delay
    if (callback)
    {
        callback();
    }
    // Derived classes (like AnimatedUnit) will override this to play animations
    // and delay the callback execution.
}

void Unit::Heal(int amount)
{
    if (!m_active || m_currentHealth <= 0)
        return;  // Cannot heal inactive/dead units

    m_currentHealth += amount;
    m_currentHealth = std::min(m_currentHealth, m_maxHealth);  // Clamp to max health
    // std::cout << m_name << " (Base) healed " << amount << " health. Health: " << m_health << "/"
    // << m_maxHealth << std::endl; // Debug
}
void Unit::AddSkill(int skillId)
{
    // Placeholder - implement properly with a Skill class/system later
    m_skills.push_back(skillId);
}
void Unit::RemoveSkill(int skillId)
{ /* Placeholder */
}
void Unit::AddEffect(int effectId)
{
    // Placeholder - implement properly with an Effect class/system later
    m_activeEffects.push_back(effectId);
}
void Unit::RemoveEffect(int effectId)
{ /* Placeholder */
}

void Unit::Reset()
{
    // TODO: remove effect first then reset this shyt
    m_currentHealth = m_maxHealth;
    m_currentMana   = m_maxMana;
    m_active        = true;  // Ensure active is true
}
