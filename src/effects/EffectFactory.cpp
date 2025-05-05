#include "effects/EffectFactory.hpp"

#include <stdexcept>

#include "effects/useEffects/ArcanePrecision.hpp"
#include "effects/useEffects/BattleFocus.hpp"
#include "effects/useEffects/Bloodthirst.hpp"
#include "effects/useEffects/Brittle.hpp"
#include "effects/useEffects/Chronoflux.hpp"
#include "effects/useEffects/CurseOfFragilityEffect.hpp"
#include "effects/useEffects/EtherealForm.hpp"
#include "effects/useEffects/FighterMight.hpp"
#include "effects/useEffects/FortuneCall.hpp"
#include "effects/useEffects/GladiatorMight.hpp"
#include "effects/useEffects/HarmoniousEmpowerment.hpp"
#include "effects/useEffects/IronCurtain.hpp"
#include "effects/useEffects/MysticBarrier.hpp"
#include "effects/useEffects/Overclock.hpp"
#include "effects/useEffects/Shadowstep.hpp"
#include "effects/useEffects/Silence.hpp"
#include "effects/useEffects/StalwartResolve.hpp"
#include "effects/useEffects/TimeWarp.hpp"

std::unique_ptr<Effect> EffectFactory::CreateUnique(const std::string& name)
{
    if (name == "ArcanePrecision")
        return std::make_unique<ArcanePrecision>(/*default args*/);
    if (name == "BattleFocus")
        return std::make_unique<BattleFocus>(/*default args*/);
    if (name == "Bloodthirst")
        return std::make_unique<Bloodthirst>(/*default args*/);
    if (name == "Brittle")
        return std::make_unique<Brittle>(/*default args*/);
    if (name == "Chronoflux")
        return std::make_unique<Chronoflux>(/*default args*/);
    if (name == "CurseOfFragility")
        return std::make_unique<CurseOfFragilityEffect>(/*default args*/);
    if (name == "EtherealForm")
        return std::make_unique<EtherealForm>(/*default args*/);
    if (name == "FighterMight")
        return std::make_unique<FighterMight>(/*default args*/);
    if (name == "FortuneCall")
        return std::make_unique<FortunesCall>(/*default args*/);
    if (name == "GladiatorMight")
        return std::make_unique<GladiatorMight>(/*default args*/);
    if (name == "HarmoniousEmpowerment")
        return std::make_unique<HarmoniousEmpowerment>(/*default args*/);
    if (name == "IronCurtain")
        return std::make_unique<IronCurtain>(/*default args*/);
    if (name == "MysticBarrier")
        return std::make_unique<MysticBarrier>(/*default args*/);
    if (name == "Overclock")
        return std::make_unique<Overclock>(/*default args*/);
    if (name == "Shadowstep")
        return std::make_unique<Shadowstep>(/*default args*/);
    if (name == "Silence")
        return std::make_unique<Silence>(/*default args*/);
    if (name == "StalwartResolve")
        return std::make_unique<StalwartResolve>(/*default args*/);
    if (name == "TimeWarp")
        return std::make_unique<TimeWarp>(/*default args*/);

    throw std::invalid_argument("Unknown effect name: " + name);
}