#pragma once

#include "skill/mobSkill/Rage.hpp"
#include "units/mobs/Mob.hpp"

class BossMob : virtual public Mob
{
public:
    BossMob(const std::string& name);

    ~BossMob() override = default;

    // Prevent copying, allow moving
    BossMob(const BossMob&)            = delete;
    BossMob& operator=(const BossMob&) = delete;
    BossMob(BossMob&&)                 = default;
    BossMob& operator=(BossMob&&)      = default;

    const Rage& GetRageSkill() const;

    int CalculateDamage(Unit& target) override;

    bool IsRageActive() const;

protected:
    bool m_isRageActive = false;

private:
    Rage m_rageSkill;  // Rage skill for the boss
};