#pragma once
#include "skill/Skill.hpp"

// Skill Slime
class AcidityCorrosion : public Skill
{
private:
    float damageMultiplier  = 0.25f;  // Silahkan nanti bisa diubah sesuai level slimenya
    int   corrosionDuration = 3;      // Duration of the corrosion effect in turns

public:
    AcidityCorrosion() : Skill("Acidity Corrosion", 0, 0, 0, 0.0f, {}, {}, false, false) {};

    void  setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
    float getDamageMultiplier() const { return damageMultiplier; }
    void  setCorrosionDuration(int duration) { corrosionDuration = duration; }
    int   getCorrosionDuration() const { return corrosionDuration; }
};