#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <functional>
#include <string>

#include "effects/Effect.hpp"
#include "effects/Stats.hpp"
#include "skill/SkillTree.hpp"

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

    virtual void Attack(Unit&                    target,
                        ActionCompletionCallback callback = nullptr,
                        ActionCompletionCallback onDeath  = nullptr);
    virtual void TakeDamage(int                      damage,
                            ActionCompletionCallback callback = nullptr,
                            ActionCompletionCallback onDeath  = nullptr);
    virtual void Heal(int amount, ActionCompletionCallback callback = nullptr);

    virtual void RestoreMana(int amount);

    virtual int CalculateDamage(Unit& target);

    /**
     * @brief Uses a skill, playing a generic skill animation.
     */
    virtual bool UseSkill(Unit&                    target,
                          ActionCompletionCallback callback = nullptr,
                          ActionCompletionCallback onDeath  = nullptr) = 0;

    virtual void AddEffect(std::unique_ptr<Effect> effect);
    virtual void RemoveEffect(const std::string& effectName);

    /**
     * @brief Resets unit state (health, mana, position, animation).
     */
    virtual void Reset();

    // --- Getters ---
    unsigned int GetId() const;

    const std::string& GetName() const;
    bool               IsActive() const;
    int                GetHealth() const;
    int                GetMaxHealth() const;
    int                GetCurrentMana() const;
    int                GetMaxMana() const;
    int                GetAttackDamage() const;
    int                GetHealthRegen() const;
    int                GetManaRegen() const;
    Stats              GetStats() const;
    float              GetMaxHealthMultiplier() const;
    float              GetHealthRegenMultiplier() const;
    float              GetMaxManaMultiplier() const;
    float              GetManaRegenMultiplier() const;
    float              GetAttackDamageMultiplier() const;
    const SkillTree*   GetSkillTree() const;

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
    void         SetMaxHealthMultiplier(float maxHealthMultiplier);
    void         SetHealthRegenMultiplier(float healthRegenMultiplier);
    void         SetMaxManaMultiplier(float maxManaMultiplier);
    void         SetManaRegenMultiplier(float manaRegenMultiplier);
    void         SetAttackDamageMultiplier(float attackDamageMultiplier);
    void         SetSkillTree(std::unique_ptr<SkillTree> skillTree);
    void         SetStats(Stats stats);

    void ApplyEffect(std::unique_ptr<Effect> effect);
    void RemoveEffectByName(const std::string& effectName);

    void RefreshTurn();

protected:
    unsigned int m_id;
    std::string  m_name;
    bool         m_active;

    // Base stats (will be overriden by level adjustments)
    int m_currentHealth = 100;
    int m_maxHealth     = 100;
    int m_healthRegen   = 0;
    int m_currentMana   = 50;
    int m_maxMana       = 50;
    int m_manaRegen     = 0;
    int m_attackDamage  = 10;

    // Multipliers
    float m_maxHealthMultiplier    = 1.0f;
    float m_healthRegenMultiplier  = 1.0f;
    float m_maxManaMultiplier      = 1.0f;
    float m_manaRegenMultiplier    = 1.0f;
    float m_attackDamageMultiplier = 1.0f;

    std::unique_ptr<SkillTree>           m_skillTree = nullptr;
    std::vector<std::unique_ptr<Effect>> m_activeEffects;
    Stats                                m_stats;

private:
    static unsigned int s_nextId;
};