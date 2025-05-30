#include "units/characters/Berseker.hpp"

#include <cmath>     // For sqrt
#include <iostream>  // For debug output

#include "skill/characterSkill/Mastery1/Fury.hpp"

Berseker::Berseker(const std::string&  name,
                   const sf::Vector2f& position,
                   NavigationGrid&     navGrid,
                   bool                isPlayerControlled,
                   const GameContext&  gameContext)
    : Unit(name),  // ← must initialize the virtual base
      Character(name),
      AnimatedUnit(name, position, navGrid, isPlayerControlled, gameContext)
{
    // Fighter-specific stat overrides
    m_maxHealth = 100;
    SetHealth(m_maxHealth);
    m_maxMana = 100;
    SetCurrentMana(m_maxMana);
    m_attackDamage = 25;
    m_healthRegen  = 3;
    m_manaRegen    = 3;
    m_moveSpeed    = 180.f;
    m_attackRange  = 48.f;

    m_skillTree = std::make_unique<SkillTree>(std::move(std::make_unique<Fury>()));

    sf::Vector2i bersekerFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> bersekerTexturePaths = {
        {UnitAnimationType::IDLE, "character_dwarf_idle"},
        {UnitAnimationType::WALK, "character_dwarf_jump"},
        {UnitAnimationType::ATTACK, "character_dwarf_attack"},
        {UnitAnimationType::CHARGED_ATTACK, "character_dwarf_charged_attack"},
        {UnitAnimationType::JUMP, "character_dwarf_jump"},
        {UnitAnimationType::DAMAGE, "character_dwarf_dmg"},
        {UnitAnimationType::DIE, "character_dwarf_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> bersekerShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_dwarf_idle_shadow"},
        {UnitAnimationType::WALK, "character_dwarf_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_dwarf_attack_shadow"},
        {UnitAnimationType::CHARGED_ATTACK, "character_dwarf_charged_attack_shadow"},
        {UnitAnimationType::JUMP, "character_dwarf_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_dwarf_dmg_shadow"},
        {UnitAnimationType::DIE, "character_dwarf_die_shadow"}

    };

    std::unordered_map<UnitAnimationType, int> bersekerFramesPerAnim = {
        {UnitAnimationType::IDLE, 16},
        {UnitAnimationType::WALK, 4},
        {UnitAnimationType::ATTACK, 4},
        {UnitAnimationType::JUMP, 4},
        {UnitAnimationType::DAMAGE, 4},
        {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> bersekerDurationPerAnim = {
        {UnitAnimationType::IDLE, 3.2f},
        {UnitAnimationType::WALK, 0.8f},
        {UnitAnimationType::ATTACK, 0.4f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 1.2f}};

    std::unordered_map<UnitAnimationType, bool> bersekerLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> bersekerDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> bersekerDefaultRows = {{UnitAnimationType::DIE, 0}};

    LoadAnimations(bersekerTexturePaths,
                   bersekerFrameSize,
                   bersekerFramesPerAnim,
                   bersekerDurationPerAnim,
                   bersekerLoopingAnims,
                   bersekerDirectionalAnims,
                   bersekerDefaultRows,
                   bersekerShadowTexturePaths);
}

void Berseker::Attack(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {  // Check self and target state
        if (callback)
            callback();  // Cannot attack, call callback immediately
        return;
    }

    AnimatedUnit* animatedTarget = dynamic_cast<AnimatedUnit*>(&target);

    sf::Vector2f vectorToTarget = animatedTarget->GetPosition() - m_position;
    float        distanceToTarget =
        std::sqrt(vectorToTarget.x * vectorToTarget.x + vectorToTarget.y * vectorToTarget.y);

    if (distanceToTarget > m_attackRange)
    {
        // Target out of range, move closer first
        // Calculate a position just within attack range
        sf::Vector2f direction       = vectorToTarget / distanceToTarget;
        sf::Vector2f positionInRange = animatedTarget->GetPosition() -
                                       direction * (m_attackRange * 0.9f);  // Move 90% of the way

        std::cout << GetName() << " moving to attack " << target.GetName() << std::endl;  // Debug

        // Move, and chain the PerformAttack call as the callback for Move
        Move(positionInRange,
             [this,
              animatedTarget,
              initialPosition = m_position,
              initialDir      = m_direction,
              cb              = std::move(callback),
              od              = std::move(onDeath)]() mutable {
                 // This lambda is called when Move() finishes (reaches destination or gets blocked)
                 sf::Vector2f vec  = animatedTarget->GetPosition() - m_position;
                 float        dist = std::sqrt(vec.x * vec.x + vec.y * vec.y);

                 if (dist <= m_attackRange * 1.1f)
                 {  // Check range again (allow small tolerance)
                     std::cout << GetName() << " reached target, performing attack on "
                               << animatedTarget->GetName() << std::endl;  // Debug

                     // Create a wrapped callback that will handle returning to position
                     // We need to wrap this in a std::shared_ptr to avoid potential lifetime issues
                     auto wrappedCallback = std::make_shared<ActionCompletionCallback>(
                         [this, initialPosition, initialDir, originalCb = std::move(cb)]() mutable {
                             std::cout << GetName()
                                       << " attack completed, returning to initial position"
                                       << std::endl;

                             // Move back to initial position
                             Move(initialPosition,
                                  [this, initialDir, originalCb = std::move(originalCb)]() mutable {
                                      // After moving back, set direction
                                      SetDirection(initialDir);

                                      // Call original callback
                                      if (originalCb)
                                          originalCb();
                                  });
                         });

                     // Call the original PerformAttack with our wrapped callback
                     // The lambda captures a shared_ptr by value to ensure it stays alive
                     this->PerformAttack(
                         *animatedTarget,
                         [wrappedCb = wrappedCallback]() {
                             // Execute our wrapped callback when PerformAttack finishes
                             if (*wrappedCb)
                                 (*wrappedCb)();
                         },
                         od);
                 }
                 else
                 {
                     std::cout << GetName()
                               << " move finished but target still out of range or blocked."
                               << std::endl;  // Debug
                     if (cb)
                         cb();  // Call original callback if attack couldn't proceed
                 }
             });
    }
    else
    {
        // Target already in range
        std::cout << GetName() << " is in range, performing attack on " << target.GetName()
                  << std::endl;  // Debug
        PerformAttack(*animatedTarget, std::move(callback), std::move(onDeath));
    }
}

void Berseker::PerformAttack(AnimatedUnit&            target,
                             ActionCompletionCallback callback,
                             ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {
        if (callback)
            callback();
        return;
    }

    // Face the target
    UpdateDirection(target.GetPosition() - m_position);

    // Play attack animation. The lambda executes *after* the animation finishes.
    PlayAnimation(
        UnitAnimationType::ATTACK,
        [this, &target, cb = std::move(callback), od = std::move(onDeath)]() mutable {
            // --- Animation Finished Callback ---
            std::cout << GetName() << "'s attack animation finished." << std::endl;  // Debug

            // Check if target is STILL valid and in range after animation delay
            if (target.IsActive() && target.GetHealth() > 0)
            {
                sf::Vector2f vectorToTarget   = target.GetPosition() - m_position;
                float        distanceToTarget = std::sqrt(vectorToTarget.x * vectorToTarget.x +
                                                   vectorToTarget.y * vectorToTarget.y);

                // Deal damage if still in range (maybe slightly larger range check here?)
                if (distanceToTarget <= m_attackRange * 1.1f)  // Allow slight tolerance
                {
                    std::cout << GetName() << " deals " << m_attackDamage << " damage to "
                              << target.GetName() << std::endl;  // Debug
                    // Cast target back to AnimatedUnit if TakeDamage is needed
                    // This is risky if target might not be AnimatedUnit, but necessary
                    // for TakeDamage
                    AnimatedUnit* animatedTarget = dynamic_cast<AnimatedUnit*>(&target);
                    if (animatedTarget)
                    {
                        animatedTarget->TakeDamage(CalculateDamage(target), nullptr, od);  // Target
                                                                                           // takes
                                                                                           // damage
                    }
                    else
                    {
                        std::cerr << "Warning: Target " << target.GetName()
                                  << " is not an AnimatedUnit, cannot TakeDamage." << std::endl;
                    }
                }
                else
                {
                    std::cout << GetName()
                              << " dealt no damage, target moved out of range during "
                                 "animation."
                              << std::endl;  // Debug
                }
            }
            else
            {
                std::cout << GetName() << " dealt no damage, target is no longer valid."
                          << std::endl;  // Debug
            }
            if (cb)
            {
                cb();
            }
        });
}

int Berseker::CalculateDamage(Unit& target)
{
    int totalDamage = static_cast<int>(
        m_attackDamage * m_attackDamageMultiplier * m_rageMultiplier + m_stats.strength);
    totalDamage *= m_stats.buffMultiplier;
    RNG   rng;
    float critChance = rng.generateProbability();
    if (critChance < m_stats.criticalStrikeChance)
    {
        totalDamage = static_cast<int>(totalDamage * m_stats.criticalStrikeMultiplier *
                                       m_stats.buffMultiplier);
    }

    totalDamage = std::max(0, totalDamage - target.GetStats().magicDefense);
    return totalDamage;
}

bool Berseker::UseSkill(Unit&                    target,
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

    BerserkerSkill* bersekerSkill = dynamic_cast<BerserkerSkill*>(selectedSkill);
    if (bersekerSkill)
    {
        m_rageAddition = bersekerSkill->getFuryMultiplier();
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << m_rageAddition;
    std::string addedText = "Rage added: " + oss.str();
    AddFloatingText(addedText, sf::Color::Yellow);

    if (callback)
        callback();

    return true;
}

void Berseker::ResetRage()
{
    m_rageAddition = 0.f;
}

void Berseker::SetLevel(int level)
{
    Character::SetLevel(level);
    m_rageMultiplier = 1.0f + (level - 1) * 0.01f;  // Example: Increase rage multiplier with level
}
// Optional: Override RenderUI if Fighter has unique elements
// void Berseker::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }
