#pragma once

#include <memory>
#include <string>

#include "effects/Effect.hpp"

class EffectFactory
{
public:
    /// create an Effect by name (throws if the name is unknown)
    static std::shared_ptr<Effect> Create(const std::string& effectName);
    static std::shared_ptr<Effect> CreateUnique(const std::string& effectName);
};