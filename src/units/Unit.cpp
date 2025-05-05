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

Stats Unit::GetStats() const
{
    return m_stats;
}

float Unit::GetMaxHealthMultiplier() const
{
    return m_maxHealthMultiplier;
}

float Unit::GetHealthRegenMultiplier() const
{
    return m_healthRegenMultiplier;
}

float Unit::GetMaxManaMultiplier() const
{
    return m_maxManaMultiplier;
}

float Unit::GetManaRegenMultiplier() const
{
    return m_manaRegenMultiplier;
}

float Unit::GetAttackDamageMultiplier() const
{
    return m_attackDamageMultiplier;
}

const SkillTree* Unit::GetSkillTree() const
{
    return m_skillTree.get();
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
    m_maxHealth     = std::max(0, maxHealth);
    m_currentHealth = std::min(m_currentHealth, m_maxHealth);

    SetHealthRegen(m_maxHealth * 0.05f * m_manaRegenMultiplier);  // 5% of max health as regen
}

void Unit::SetHealth(int health)
{
    m_currentHealth = std::max(0, std::min(m_maxHealth, health));
}

void Unit::SetMaxMana(int maxMana)
{
    m_maxMana     = std::max(0, maxMana);
    m_currentMana = std::min(m_currentMana, m_maxMana);

    SetManaRegen(m_maxHealth * 0.05f * m_manaRegenMultiplier);  // 5% of max mana as regen
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

void Unit::SetMaxHealthMultiplier(float maxHealthMultiplier)
{
    m_maxHealthMultiplier = maxHealthMultiplier;
    m_maxHealth           = static_cast<int>(m_maxHealth * maxHealthMultiplier);
}
void Unit::SetHealthRegenMultiplier(float healthRegenMultiplier)
{
    m_healthRegenMultiplier = healthRegenMultiplier;
    m_healthRegen           = static_cast<int>(m_healthRegen * healthRegenMultiplier);
}
void Unit::SetMaxManaMultiplier(float maxManaMultiplier)
{
    m_maxManaMultiplier = maxManaMultiplier;
    m_maxMana           = static_cast<int>(m_maxMana * maxManaMultiplier);
}
void Unit::SetManaRegenMultiplier(float manaRegenMultiplier)
{
    m_manaRegenMultiplier = manaRegenMultiplier;
    m_manaRegen           = static_cast<int>(m_manaRegen * manaRegenMultiplier);
}
void Unit::SetAttackDamageMultiplier(float attackDamageMultiplier)
{
    m_attackDamageMultiplier = attackDamageMultiplier;
    m_attackDamage           = static_cast<int>(m_attackDamage * attackDamageMultiplier);
}
void Unit::SetSkillTree(std::unique_ptr<SkillTree> skillTree)
{
    m_skillTree = std::move(skillTree);
}
void Unit::SetStats(Stats stats)
{
    m_stats = stats;
}

void Unit::ApplyEffect(std::unique_ptr<Effect> effect)
{
    if (!m_active || m_currentHealth <= 0)
        return;  // Cannot apply effects to inactive/dead units

    // Apply the effect to the unit
    m_activeEffects.push_back(std::move(effect));
}

void Unit::RemoveEffectByName(const std::string& effectName)
{
    if (!m_active || m_currentHealth <= 0)
        return;  // Cannot remove effects from inactive/dead units

    auto it = std::remove_if(
        m_activeEffects.begin(), m_activeEffects.end(), [&](const std::unique_ptr<Effect>& effect) {
            return effect->GetName() == effectName;
        });
    m_activeEffects.erase(it, m_activeEffects.end());
}

// --- Base implementations for actions affecting stats ---

void Unit::Attack(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();  // Already inactive, just run callback
        return;
    }

    // Basic attack logic: reduce target's health by this unit's attack damage
    target.TakeDamage(CalculateDamage(target), callback, onDeath);
}

int Unit::CalculateDamage(Unit& target)
{
    return m_attackDamage * m_attackDamageMultiplier;
}

void Unit::TakeDamage(int damage, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
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
        if (onDeath)
        {
            onDeath();
        }
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

void Unit::Heal(int amount, ActionCompletionCallback callback)
{
    if (!m_active || m_currentHealth <= 0)
        return;  // Cannot heal inactive/dead units

    m_currentHealth += amount;
    m_currentHealth = std::min(m_currentHealth, m_maxHealth);  // Clamp to max health
    // std::cout << m_name << " (Base) healed " << amount << " health. Health: " << m_health << "/"
    // << m_maxHealth << std::endl; // Debug
    if (callback)
    {
        callback();
    }
}
void Unit::AddEffect(std::unique_ptr<Effect> effect)
{
    if (!m_active || m_currentHealth <= 0)
        return;  // Cannot add effects to inactive/dead units

    // Find same effect if so add duration.
    for (auto& effectIt : m_activeEffects)
    {
        if (effectIt->GetName() == effect->GetName())
        {
            effectIt->SetRemainingDuration(effectIt->GetRemainingDuration() + effect->GetDuration());
            return;
        }
    }

    m_stats += effect->GetModifiers();

    m_activeEffects.push_back(std::move(effect));
}

void Unit::RemoveEffect(const std::string& effectName)
{
    if (!m_active || m_currentHealth <= 0)
        return;

    for (auto it = m_activeEffects.begin(); it != m_activeEffects.end();)
    {
        if ((*it)->GetName() == effectName)
        {
            m_stats -= (*it)->GetModifiers();
            it = m_activeEffects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Unit::Reset()
{
    for (auto& effect : m_activeEffects)
    {
        m_stats -= effect->GetModifiers();
    }
    m_activeEffects.clear();
    m_currentHealth = m_maxHealth;
    m_currentMana   = m_maxMana;
    m_active        = true;  // Ensure active is true
}

void Unit::RefreshTurn()
{
    if (!m_active || m_currentHealth <= 0)
        return;

    for (auto it = m_activeEffects.begin(); it != m_activeEffects.end();)
    {
        (*it)->SetRemainingDuration((*it)->GetRemainingDuration() - 1);
        if ((*it)->GetRemainingDuration() <= 0)
        {
            m_stats -= (*it)->GetModifiers();
            it = m_activeEffects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (m_currentHealth < m_maxHealth)
    {
        int regenAmount = static_cast<int>(m_healthRegen * m_healthRegenMultiplier);
        regenAmount     = std::min(regenAmount, m_maxHealth - m_currentHealth);
        Heal(regenAmount);
    }

    if (m_currentMana < m_maxMana)
    {
        int manaRegenAmount = static_cast<int>(m_manaRegen * m_manaRegenMultiplier);
        manaRegenAmount     = std::min(manaRegenAmount, m_maxMana - m_currentMana);
        RestoreMana(manaRegenAmount);
    }
}