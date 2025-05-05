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
    m_summonedUnits = 1;

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
    // Early‑out if we can't act or target is invalid
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {
        if (callback)
            callback();
        return;
    }

    // Capture the actual target pointer so we don't reference a dead stack frame
    Unit* targetPtr = &target;

    // --- If we have no summons, try summoning ---
    if (m_summons.empty())
    {
        // Prepare a callback that runs after the jump animation
        std::function<void()> jumpCallback = [this,
                                              targetPtr,
                                              summonCount = m_summonedUnits,
                                              chanceProb  = m_summonChance,
                                              cb          = std::move(callback),
                                              od          = std::move(onDeath)]() mutable {
            RNG rng;
            if (rng.generateProbability() < chanceProb)
            {
                // Generate spawn points
                sf::Vector2f basePos = m_position;
                basePos.x += 100;
                auto spawnPts = Summon::generateSummonSpawnPoints(basePos, summonCount, true);

                // Summon each Zombie exactly once
                for (int i = 0; i < summonCount; ++i)
                {
                    auto z = std::make_unique<Zombie>(
                        "Zombie", spawnPts[i], m_navGrid, false, m_gameContext);
                    z->SetScale(8.f, 8.f);
                    z->SetActive(true);
                    z->SetControlledByPlayer(false);
                    z->SetNavGrid(m_navGrid);
                    z->SetShowUI(true);
                    z->SetLevel(m_level);
                    z->SetDirection(Direction::EAST);

                    // Keep the raw pointer so we can grab its ID after registration
                    Zombie* rawZ = z.get();
                    m_gameContext.GetUnitManager()->AddUnitImmediate(std::move(z));
                    m_summons.push_back(rawZ->GetId());
                }

                std::cout << "Summoned " << summonCount << " zombies!" << std::endl;
            }
            else
            {
                AddFloatingText("Summon Failed", sf::Color::Red);
            }
            // In either case, call the callback so the mage's turn can finish
            if (cb)
                cb();
        };

        // Play the jump animation, then run our summoning logic
        PlayAnimation(UnitAnimationType::JUMP, std::move(jumpCallback));
        return;
    }

    // --- Otherwise, we already have summons: let them attack ---

    // First: remove dead summons from the list
    bool anyAlive = false;
    for (auto it = m_summons.begin(); it != m_summons.end();)
    {
        if (auto* u = m_gameContext.GetUnitManager()->GetUnit(*it);
            u && u->IsActive() && u->GetHealth() > 0)
        {
            anyAlive = true;
            ++it;
        }
        else
        {
            it = m_summons.erase(it);
        }
    }

    // If all summons died, clear and retry summoning
    if (!anyAlive)
    {
        m_summons.clear();
        // Retry attack, which will go into the summon‑branch
        Attack(target, std::move(callback), std::move(onDeath));
        return;
    }

    // Play a quick jump to signal the attack phase
    PlayAnimation(UnitAnimationType::JUMP);

    // Setup the counter and final callback
    size_t total   = m_summons.size();
    auto   counter = std::make_shared<std::atomic_int>(0);
    auto   finalCb = std::move(callback);

    auto makeOnDamageDone = [counter, total, finalCb]() mutable {
        int done = counter->fetch_add(1, std::memory_order_relaxed) + 1;
        std::cout << "Summon damage done: " << done << "/" << total << std::endl;
        if (done == (int)total && finalCb)
        {
            finalCb();
        }
    };

    auto wrappedOnDeath = [makeOnDamageDone, od = std::move(onDeath)]() mutable {
        makeOnDamageDone();
        if (od)
            od();
    };

    // Instruct each zombie to attack
    for (int id : m_summons)
    {
        if (auto* s = m_gameContext.GetUnitManager()->GetUnit(id);
            s && s->IsActive() && targetPtr && targetPtr->IsActive())
        {
            s->Attack(*targetPtr, makeOnDamageDone, wrappedOnDeath);
        }
        else
        {
            std::cerr << "Skipping attack for dead/missing summon " << id << "\n";
        }
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

bool Necromancer::UseSkill(Unit&                    target,
                           ActionCompletionCallback callback,
                           ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return false;
    }

    RNG                 rng;
    int                 totalManaCost = 0;
    std::vector<Skill*> activeSkills  = m_skillTree->getActiveSkill();

    // filter by mana cost lower or equal to current mana
    std::vector<Skill*> filteredSkills;
    for (auto& skill : activeSkills)
    {
        if (skill->getManaCost() <= m_currentMana)
        {
            filteredSkills.push_back(skill);
        }
    }

    if (filteredSkills.empty())
    {
        AddFloatingText("Not enough mana", sf::Color::Yellow);  // Red text for no mana
        return false;
    }

    int    randomIndex   = rng.generateInRange(0, filteredSkills.size() - 1);
    Skill* selectedSkill = filteredSkills[randomIndex];
    totalManaCost        = selectedSkill->getManaCost();
    m_currentMana -= totalManaCost;

    float effectChance = rng.generateProbability();
    if (effectChance < selectedSkill->getEffectChance())
    {
        AddFloatingText("Effect applied", sf::Color::Green);
        // copy the unique ptr
        for (auto& effect : selectedSkill->getEffects())
        {
            std::unique_ptr<Effect> effectTemp = std::make_unique<Effect>(*effect);
            AddEffect(std::move(effectTemp));
        }
    }

    float             lifeStealPercentage = m_lifestealPercentage;
    NecromancerSkill* necromancerSkill    = dynamic_cast<NecromancerSkill*>(selectedSkill);
    if (necromancerSkill)
    {
        lifeStealPercentage += necromancerSkill->getLifestealPercentage();
    }

    int targetLastHealth = target.GetHealth();

    auto wrappedCallback = [this, &target, targetLastHealth, lifeStealPercentage, callback]() mutable {
        if (!target.IsActive() || target.GetHealth() <= 0)
        {
            if (callback)
                callback();
        }
        int damageDone = targetLastHealth - target.GetHealth();
        if (damageDone > 0)
        {
            Heal(damageDone);
        }
        if (callback)
            callback();
    };

    auto wrappedOnDeath = [this, &target, targetLastHealth, lifeStealPercentage, onDeath]() mutable {
        if (!target.IsActive() || target.GetHealth() <= 0)
        {
            int damageDone = targetLastHealth - target.GetHealth();
            if (damageDone > 0)
            {
                Heal(damageDone);
            }
        }
        if (onDeath)
            onDeath();
    };

    target.TakeDamage(target.GetHealth() * lifeStealPercentage, wrappedCallback, wrappedCallback);

    return true;
}

void Necromancer::SetLevel(int level)
{
    Character::SetLevel(level);
    m_summonedUnits = 1 + level / 40;
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