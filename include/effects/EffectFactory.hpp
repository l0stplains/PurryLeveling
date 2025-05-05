#pragma once

#include <memory>
#include <string>

#include "effects/Effect.hpp"

class EffectFactory
{
public:
    static std::unique_ptr<Effect> CreateUnique(const std::string& effectName);
};