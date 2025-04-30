#pragma once
#include <string>

#include "effects/Stats.hpp"

class Effect
{
public:
    Effect(const std::string& name, const std::string& description, int duration);
    virtual ~Effect() = default;

    // Prevent copying and assignment
    Effect(const Effect&)            = delete;
    Effect& operator=(const Effect&) = delete;
    Effect(Effect&&)                 = default;
    Effect& operator=(Effect&&)      = default;

    // --- Getters ---
    const std::string& GetName() const { return m_name; }
    const std::string& GetDescription() const { return m_description; }
    int                GetDuration() const { return m_duration; }
    int                GetRemainingDuration() const { return m_remainingDuration; }

    virtual void ApplyEffect(unsigned int unitID);  // on integration will be applied to unit
    virtual void RemoveEffect(unsigned int unitID);

protected:
    std::string m_name;
    std::string m_description;
    int         m_duration;  // in turns
    int         m_remainingDuration;
    Stats       m_modifiers;
};