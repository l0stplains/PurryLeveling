#include "effects/Effect.hpp"

Effect::Effect(const std::string& name, const std::string& description, int duration)
    : m_name(name), m_description(description), m_duration(duration), m_remainingDuration(duration)
{}

void Effect::ApplyEffect(unsigned int unitID) {}

void Effect::RemoveEffect(unsigned int unitID) {}