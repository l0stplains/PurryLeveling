#pragma once

#include "units/Unit.hpp"

class Summon : virtual public Unit
{
public:
    Summon(const std::string& name);

    ~Summon() override = default;

    Summon(const Summon&)            = delete;
    Summon& operator=(const Summon&) = delete;
    Summon(Summon&&)                 = default;
    Summon& operator=(Summon&&)      = default;

    int  GetLevel() const;
    void SetLevel(int);

protected:
    int m_level = 1;
};