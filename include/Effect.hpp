#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <string>
using namespace std;

class Effect
{
    private:
        string name;
        string description;
        float duration;
        float remainingDuration;

    public:
        Effect(string name, string description, float duraition);
        Effect(const Effect& other);
        ~Effect();
        void apply();
        void remove();
};

#endif // EFFECT_HPP