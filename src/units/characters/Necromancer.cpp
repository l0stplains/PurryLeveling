#include "units/characters/Necromancer.hpp"

#include <atomic>
#include <cmath>     // For sqrt
#include <iostream>  // For debug output

#include "rng/rng.hpp"
#include "skill/characterSkill/Mastery1/LifeSteal.hpp"
#include "units/summons/Summon.hpp"
#include "units/summons/Zombie.hpp"

Necromancer::Necromancer(const std::string&  name,
                         const sf::Vector2f& position,
                         NavigationGrid&     navGrid,
                         bool                isPlayerControlled,
                         const GameContext&  gameContext)
    : Unit(name),  // ← must initialize the virtual base
      Character(name),
      AnimatedUnit(name, position, navGrid, isPlayerControlled, gameContext)
{
    m_summonedUnits = 2;

    m_skillTree = std::make_unique<SkillTree>(std::move(std::make_unique<LifeSteal>()));

    sf::Vector2i                                       necromancerFrameSize(32, 32);
    std::unordered_map<UnitAnimationType, std::string> necromancerTexturePaths = {
        {UnitAnimationType::IDLE, "character_cyclop_idle"},
        {UnitAnimationType::WALK, "character_cyclop_jump"},
        {UnitAnimationType::ATTACK, "character_cyclop_attack"},
        {UnitAnimationType::JUMP, "character_cyclop_jump"},
        {UnitAnimationType::DAMAGE, "character_cyclop_dmg"},
        {UnitAnimationType::DIE, "character_cyclop_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> necromancerShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_cyclop_idle_shadow"},
        {UnitAnimationType::WALK, "character_cyclop_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_cyclop_attack_shadow"},
        {UnitAnimationType::JUMP, "character_cyclop_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_cyclop_dmg_shadow"},
        {UnitAnimationType::DIE, "character_cyclop_die_shadow"}

    };

    std::unordered_map<UnitAnimationType, int> necromancerFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> necromancerDurationPerAnim = {
        {UnitAnimationType::IDLE, 3.2f},
        {UnitAnimationType::WALK, 0.8f},
        {UnitAnimationType::ATTACK, 0.4f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 1.2f}};

    std::unordered_map<UnitAnimationType, bool> necromancerLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> necromancerDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> necromancerDefaultRows = {{UnitAnimationType::DIE, 0}};

    LoadAnimations(necromancerTexturePaths,
                   necromancerFrameSize,
                   necromancerFramesPerAnim,
                   necromancerDurationPerAnim,
                   necromancerLoopingAnims,
                   necromancerDirectionalAnims,
                   necromancerDefaultRows,
                   necromancerShadowTexturePaths);
}

void Necromancer::Attack(Unit&                    target,
                         ActionCompletionCallback callback,
                         ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {  // Check self and target state
        if (callback)
            callback();  // Cannot attack, call callback immediately
        return;
    }
    // if no summons, summon a unit
    if (m_summons.size() == 0)
    {
        std::function<void()> jumpCallback = [this, callback, onDeath]() {
            RNG   rng;
            float chance = rng.generateProbability();
            if (chance < m_summonChance)
            {
                sf::Vector2f summonPosition = m_position;
                summonPosition.x += 100;
                std::vector<sf::Vector2f> summonPositions =
                    Summon::generateSummonSpawnPoints(summonPosition, m_summonedUnits, true);
                for (int i = 0; i < m_summonedUnits; i++)
                {
                    std::unique_ptr<Zombie> zombie = make_unique<Zombie>(
                        std::string("Zombie"), summonPositions[i], m_navGrid, false, m_gameContext);
                    zombie->SetScale(8.f, 8.f);
                    zombie->SetActive(true);
                    zombie->SetControlledByPlayer(false);
                    zombie->SetNavGrid(m_navGrid);
                    zombie->SetShowUI(true);
                    zombie->SetLevel(m_level);
                    zombie->SetDirection(Direction::EAST);

                    Zombie* raw = zombie.get();

                    // register in the manager
                    m_gameContext.GetUnitManager()->AddUnit(std::move(zombie));

                    // now the manager “knows” about that ID
                    m_summons.push_back(raw->GetId());

                    m_summons.push_back(zombie->GetId());
                    m_gameContext.GetUnitManager()->AddUnit(std::move(zombie));
                }
                std::cout << "Summoned " << m_summonedUnits << " zombies!" << std::endl;
                if (callback)
                    callback();  // Cannot attack, call callback immediately
                return;
            }
            if (callback)
                callback();  // Cannot summon, call callback immediately
            return;
        };
        PlayAnimation(UnitAnimationType::JUMP, jumpCallback);
    }
    else
    {
        PlayAnimation(UnitAnimationType::JUMP);
        // Check if any summons are still alive
        bool   allDead    = true;
        size_t totalAlive = 0;  // capture size now, in case m_summons changes later
        for (const auto& summonId : m_summons)
        {
            Unit* summon = m_gameContext.GetUnitManager()->GetUnit(summonId);
            if (summon && summon->IsActive() && summon->GetHealth() > 0)
            {
                allDead = false;
                ++totalAlive;  // count alive summons
            }
        }

        if (allDead)
        {
            m_summons.clear();                    // Clear the list if all summons are dead
            Attack(target, std::move(callback));  // Retry the attack
            return;
        }

        Unit*  targetPtr = &target;  // take address of the abstract base
        int    damage    = m_attackDamage;
        size_t total     = m_summons.size();
        auto   counter   = std::make_shared<std::atomic_int>(0);
        auto   finalCb   = std::move(callback);

        // helper to bump the count and call finalCb at the end
        auto makeOnDamageDone = [this, counter, total, finalCb]() mutable {
            std::cout << "Summon damage done, total: " << counter->load() << std::endl;
            int done = counter->fetch_add(1, std::memory_order_relaxed) + 1;
            if (done == (int)total && finalCb)
            {
                finalCb();
            }
        };

        auto wrappedOnDeath = [this, makeOnDamageDone, onDeath]() mutable {
            makeOnDamageDone();
            if (onDeath)
            {
                onDeath();
            }
        };

        for (auto summonId : m_summons)
        {
            if (auto s = m_gameContext.GetUnitManager()->GetUnit(summonId);
                s && s->IsActive() && s->GetHealth() > 0)
            {
                // first, attack animation; when it finishes we apply damage
                s->Attack(*targetPtr, makeOnDamageDone, wrappedOnDeath);
            }
        }

        return;
    }
}

void Necromancer::KillSummons(ActionCompletionCallback callback)
{
    if (m_summons.empty())
    {
        if (callback)
            callback();
        return;
    }

    for (const auto& summonId : m_summons)
    {
        Unit* summon = m_gameContext.GetUnitManager()->GetUnit(summonId);
        if (summon && summon->IsActive() && summon->GetHealth() > 0)
        {
            summon->TakeDamage(summon->GetHealth(), callback);
        }
    }
    m_summons.clear();
}

void Necromancer::UseSkill(Unit&                    target,
                           ActionCompletionCallback callback,
                           ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return;
    }
}

const std::vector<unsigned int>& Necromancer::GetSummons() const
{
    return m_summons;
}

// Optional: Override RenderUI if Fighter has unique elements
// void Necromancer::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }

float Necromancer::GetSummonChance() const
{
    return m_summonChance;
}
int Necromancer::GetSummonedUnits() const
{
    return m_summonedUnits;
}
float Necromancer::GetLifestealPercentage() const
{
    return m_lifestealPercentage;
}
void Necromancer::SetSummonChance(float chance)
{
    m_summonChance = chance;
}
void Necromancer::SetSummonedUnits(int units)
{
    m_summonedUnits = units;
}
void Necromancer::SetLifestealPercentage(float percentage)
{
    m_lifestealPercentage = percentage;
}