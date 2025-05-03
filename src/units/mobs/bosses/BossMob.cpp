#include "units/mobs/bosses/BossMob.hpp"

BossMob::BossMob(const std::string& name) : Mob(name), m_rageSkill(true, true) {}

const Rage& BossMob::GetRageSkill() const
{
    return m_rageSkill;
}