#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <functional>
#include <string>
#include "effects/Stats.hpp"

/**
 * @brief Abstract base class for any entity in the game world.
 * Now includes core stats common to interactive units.
 */
class Unit
{
public:
    using ActionCompletionCallback = std::function<void()>;

    Unit(const std::string& name);
    virtual ~Unit() = default;

    // Prevent copying and assignment
    Unit(const Unit&)            = delete;
    Unit& operator=(const Unit&) = delete;
    Unit(Unit&&)                 = default;
    Unit& operator=(Unit&&)      = default;

    virtual void Attack(Unit& target, ActionCompletionCallback callback = nullptr);
    virtual void TakeDamage(int damage, ActionCompletionCallback callback = nullptr);
    virtual void Heal(int amount);

    virtual void RestoreMana(int amount);

    /**
     * @brief Uses a skill, playing a generic skill animation.
     */
    virtual void UseSkill(int skillId, ActionCompletionCallback callback = nullptr) = 0;

    // AddSkill, RemoveSkill, AddEffect, RemoveEffect placeholders (virtual if needed)
    virtual void AddSkill(int skillId);
    virtual void RemoveSkill(int skillId);
    virtual void AddEffect(int effectId);
    virtual void RemoveEffect(int effectId);

    /**
     * @brief Resets unit state (health, mana, position, animation).
     */
    virtual void Reset();

    // --- Getters ---
    unsigned int       GetId() const;
    const std::string& GetName() const;
    bool               IsActive() const;
    int                GetHealth() const;
    int                GetMaxHealth() const;
    int                GetCurrentMana() const;
    int                GetMaxMana() const;
    int                GetAttackDamage() const;
    int                GetHealthRegen() const;
    int                GetManaRegen() const;

    // --- Setters ---
    void         SetName(const std::string& name);
    void         SetActive(bool active);
    virtual void SetMaxHealth(int maxHealth);
    virtual void SetHealth(int health);
    virtual void SetMaxMana(int maxMana);
    virtual void SetCurrentMana(int currentMana);
    virtual void SetAttackDamage(int attackDamage);
    virtual void SetHealthRegen(int healthRegen);
    virtual void SetManaRegen(int manaRegen);

protected:
    unsigned int m_id;
    std::string  m_name;
    bool         m_active;

    // --- Core Stats moved back to Unit ---
    int m_currentHealth = 100;
    int m_maxHealth     = 100;
    int m_healthRegen   = 0;  // Default to no regen unless set
    int m_currentMana   = 50;
    int m_maxMana       = 50;
    int m_manaRegen     = 0;   // Default to no regen unless set
    int m_attackDamage  = 10;  // Base damage

    // --- Skills & Effects (Placeholders) ---
    std::vector<int> m_skills;  // Consider dedicated Skill/Effect classes later
    std::vector<int> m_activeEffects;
    Stats m_stats;  // Core stats (health, mana, etc.)

private:
    static unsigned int s_nextId;
};