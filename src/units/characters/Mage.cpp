#include "units/characters/Mage.hpp"

#include <atomic>
#include <cmath>     // For sqrt
#include <iostream>  // For debug output
#include <limits>

#include "skill/characterSkill/Mastery1/Focus.hpp"
#include "units/summons/Summon.hpp"
#include "units/summons/Wildfire.hpp"

Mage::Mage(const std::string&  name,
           const sf::Vector2f& position,
           NavigationGrid&     navGrid,
           bool                isPlayerControlled,
           const GameContext&  gameContext)
    : Unit(name),  // ← must initialize the virtual base
      Character(name),
      AnimatedUnit(name, position, navGrid, isPlayerControlled, gameContext)
{
    m_summonedUnits = 4;

    m_skillTree = std::make_unique<SkillTree>(std::move(std::make_unique<Focus>()));

    sf::Vector2i mageFrameSize(32, 32);

    std::unordered_map<UnitAnimationType, std::string> mageTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle"},
        {UnitAnimationType::WALK, "character_centaur_jump"},
        {UnitAnimationType::ATTACK, "character_centaur_attack"},
        {UnitAnimationType::JUMP, "character_centaur_jump"},
        {UnitAnimationType::DAMAGE, "character_centaur_dmg"},
        {UnitAnimationType::DIE, "character_centaur_die"}

    };
    std::unordered_map<UnitAnimationType, std::string> mageShadowTexturePaths = {
        {UnitAnimationType::IDLE, "character_centaur_idle_shadow"},
        {UnitAnimationType::WALK, "character_centaur_jump_shadow"},
        {UnitAnimationType::ATTACK, "character_centaur_attack_shadow"},
        {UnitAnimationType::JUMP, "character_centaur_jump_shadow"},
        {UnitAnimationType::DAMAGE, "character_centaur_dmg_shadow"},
        {UnitAnimationType::DIE, "character_centaur_die_shadow"}

    };
    std::unordered_map<UnitAnimationType, int> mageFramesPerAnim = {{UnitAnimationType::IDLE, 16},
                                                                    {UnitAnimationType::WALK, 4},
                                                                    {UnitAnimationType::ATTACK, 4},
                                                                    {UnitAnimationType::JUMP, 4},
                                                                    {UnitAnimationType::DAMAGE, 4},
                                                                    {UnitAnimationType::DIE, 12}};

    std::unordered_map<UnitAnimationType, float> mageDurationPerAnim = {
        {UnitAnimationType::IDLE, 3.2f},
        {UnitAnimationType::WALK, 0.8f},
        {UnitAnimationType::ATTACK, 0.4f},
        {UnitAnimationType::JUMP, 0.4f},
        {UnitAnimationType::DAMAGE, 0.4f},
        {UnitAnimationType::DIE, 1.2f}};

    std::unordered_map<UnitAnimationType, bool> mageLoopingAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, false},
        {UnitAnimationType::JUMP, false},
        {UnitAnimationType::DAMAGE, false},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, bool> mageDirectionalAnims = {
        {UnitAnimationType::IDLE, true},
        {UnitAnimationType::WALK, true},
        {UnitAnimationType::ATTACK, true},
        {UnitAnimationType::JUMP, true},
        {UnitAnimationType::DAMAGE, true},
        {UnitAnimationType::DIE, false}};

    std::unordered_map<UnitAnimationType, int> mageDefaultRows = {{UnitAnimationType::DIE, 0}};

    LoadAnimations(mageTexturePaths,
                   mageFrameSize,
                   mageFramesPerAnim,
                   mageDurationPerAnim,
                   mageLoopingAnims,
                   mageDirectionalAnims,
                   mageDefaultRows,
                   mageShadowTexturePaths);
}

void Mage::Attack(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0 || !target.IsActive())
    {  // Check self and target state
        if (callback)
            callback();  // Cannot attack, call callback immediately
        return;
    }

    // clean all summoned Wildfires before summoning new ones
    for (const auto& summonId : m_summons)
    {
        std::vector<Wildfire*> summonedWildfires =
            m_gameContext.GetUnitManager()->GetAllUnitsOfType<Wildfire>();
        for (auto& wildfire : summonedWildfires)
        {
            if (wildfire->GetId() == summonId)
            {
                m_gameContext.GetUnitManager()->RemoveUnit(wildfire->GetId());
                std::cout << "Removed Wildfire with ID: " << wildfire->GetId() << std::endl;
            }
        }
    }
    std::function<void()> jumpCallback = [this, &target, callback, onDeath]() {
        sf::Vector2f summonPosition = m_position;
        summonPosition.x += 100;
        std::vector<sf::Vector2f> summonPositions =
            Summon::generateSummonSpawnPoints(summonPosition, m_summonedUnits, true);
        for (int i = 0; i < m_summonedUnits; i++)
        {
            std::unique_ptr<Wildfire> wildfire = make_unique<Wildfire>(
                std::string("Wildfire"), summonPositions[i], m_navGrid, false, m_gameContext);
            wildfire->SetScale(5.f, 5.f);
            wildfire->SetActive(true);
            wildfire->SetControlledByPlayer(false);
            wildfire->SetNavGrid(m_navGrid);
            wildfire->SetShowUI(false);
            wildfire->SetLevel(m_level);
            wildfire->SetDirection(Direction::EAST);

            // i spent like 6 hours trying to figure out a race condition issue here
            Wildfire* raw = wildfire.get();

            // register in the manager
            m_gameContext.GetUnitManager()->AddUnit(std::move(wildfire));

            // now the manager “knows” about that ID
            m_summons.push_back(raw->GetId());
        }

        std::cout << "Summoned " << m_summonedUnits << " wildfires!" << std::endl;

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
                m_summons.clear();
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
    };
    PlayAnimation(UnitAnimationType::JUMP, jumpCallback);
    return;
}

void Mage::UseSkill(Unit& target, ActionCompletionCallback callback, ActionCompletionCallback onDeath)
{
    if (!m_active || m_currentHealth <= 0)
    {
        if (callback)
            callback();
        return;
    }
}

// Optional: Override RenderUI if Fighter has unique elements
// void Mage::RenderUI(sf::RenderWindow& window) {
//     Character::RenderUI(window); // Call base UI rendering
//     // Add fighter-specific UI elements here (e.g., rage bar?)
// }