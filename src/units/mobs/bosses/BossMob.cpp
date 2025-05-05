#include "units/mobs/bosses/BossMob.hpp"

#include "rng/rng.hpp"

BossMob::BossMob(const std::string& name) : Mob(name), m_rageSkill(true, true)
{
    m_stats.dodgeChance              = 0.1f;
    m_stats.criticalStrikeChance     = 0.2f;
    m_stats.criticalStrikeMultiplier = 1.5f;
}

const Rage& BossMob::GetRageSkill() const
{
    return m_rageSkill;
}

int BossMob::CalculateDamage(Unit& target)
{
    int damage = static_cast<int>(m_attackDamage * m_attackDamageMultiplier);
    if (m_isRageActive)
    {
        damage = static_cast<int>(damage * m_rageSkill.getRageMultiplier());
    }
    RNG   rng;
    float critChance = rng.generateProbability();
    if (critChance < m_stats.criticalStrikeChance)
    {
        damage = static_cast<int>(damage * m_stats.criticalStrikeMultiplier * m_stats.buffMultiplier);
    }
    return damage;
}

bool BossMob::IsRageActive() const
{
    return m_isRageActive;
}