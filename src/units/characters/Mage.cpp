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
    {
        if (callback)
            callback();
        return;
    }

    for (int oldId : m_summons)
    {
        m_gameContext.GetUnitManager()->RemoveUnit(oldId);
        std::cout << "Removed Wildfire with ID: " << oldId << std::endl;
    }
    m_summons.clear();

    Unit* targetPtr = &target;

    std::function<void()> jumpCallback = [this,
                                          targetPtr,
                                          damage  = m_attackDamage,
                                          summons = &m_summons,  // capture pointer to the vector
                                          navGrid = &m_navGrid,
                                          lvl     = m_level,
                                          cb      = std::move(callback),
                                          od      = std::move(onDeath)]() mutable {
        // generate spawn points
        sf::Vector2f basePos = m_position;
        basePos.x += 100;
        auto positions = Summon::generateSummonSpawnPoints(basePos, m_summonedUnits, true);

        // summon new Wildfires
        for (size_t i = 0; i < positions.size(); ++i)
        {
            auto wf =
                std::make_unique<Wildfire>("Wildfire", positions[i], *navGrid, false, m_gameContext);
            wf->SetScale(5.f, 5.f);
            wf->SetActive(true);
            wf->SetControlledByPlayer(false);
            wf->SetNavGrid(*navGrid);
            wf->SetShowUI(false);
            wf->SetLevel(lvl);
            wf->SetDirection(Direction::EAST);

            Wildfire* raw = wf.get();
            m_gameContext.GetUnitManager()->AddUnit(std::move(wf));
            summons->push_back(raw->GetId());
        }

        std::cout << "Summoned " << summons->size() << " wildfires!" << std::endl;

        size_t total   = summons->size();
        auto   counter = std::make_shared<std::atomic_int>(0);

        // damage‐done helper
        auto makeOnDamageDone = [summons, counter, total, cb]() mutable {
            int done = counter->fetch_add(1, std::memory_order_relaxed) + 1;
            std::cout << "Summon damage done, total: " << done << "/" << total << std::endl;
            if (done == (int)total && cb)
            {
                summons->clear();
                cb();
            }
        };

        auto wrappedOnDeath = [makeOnDamageDone, od]() mutable {
            makeOnDamageDone();
            if (od)
                od();
        };

        // start each Wildfire’s attack
        for (int id : *summons)
        {
            if (auto* s = m_gameContext.GetUnitManager()->GetUnit(id);
                s && targetPtr && targetPtr->IsActive())
            {
                std::cout << "Attacking with Wildfire ID: " << id << std::endl;
                s->Attack(*targetPtr, makeOnDamageDone, wrappedOnDeath);
            }
            else
            {
                std::cerr << "Skipping attack for missing or dead unit " << id << "\n";
            }
        }
    };

    PlayAnimation(UnitAnimationType::JUMP, std::move(jumpCallback));
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