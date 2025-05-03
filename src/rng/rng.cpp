#include "rng/rng.hpp"

usl RNG::state = 0;

RNG::RNG()
{
    if (state == 0)
    {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        state    = static_cast<usl>(now);
    }
}

RNG::RNG(usl seed)
{
    if (state == 0)
    {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        state    = static_cast<usl>(now);
    }
    setSeed(seed);
}

void RNG::setSeed(usl seed)
{
    generateNext();
    state += seed;
    generateNext();
}

RNG& RNG::operator=(const RNG& other)
{
    if (this != &other)
    {
        this->state = other.state;
    }
    return *this;
}

RNG::~RNG() {}

unsigned int RNG::generateNext()
{
    usl oldState = state;

    state = oldState * multiplier + increment;

    unsigned int shifted = ((oldState >> 18) ^ oldState) >> 27;
    unsigned int rot     = oldState >> 59;

    return (shifted >> rot) | (shifted << ((-rot) & 31));
}

unsigned int RNG::generateInRange(unsigned int max)
{
    if (max == 0)
        return 0;

    unsigned int threshold = -max % max;

    while (true)
    {
        unsigned int r = generateNext();
        if (r >= threshold)
        {
            return r % max;
        }
    }
}

unsigned int RNG::generateInRange(unsigned int min, unsigned int max)
{
    if (min >= max)
        return min;

    return min + generateInRange(max - min + 1);
}

double RNG::generateProbability()
{
    return generateNext() / (double)4294967296;  // 2^32
}

double RNG::generatePercentage()
{
    return generateProbability() * 100.0;
}

// Vector of random numbers
vector<unsigned int> RNG::generateRandomVector(size_t size, unsigned int max)
{
    vector<unsigned int> randomVector;
    randomVector.reserve(size);

    for (size_t i = 0; i < size; i++)
    {
        randomVector.push_back(generateInRange(max));
    }

    return randomVector;
}

// Vector of percentages
vector<double> RNG::generatePercentageVector(size_t size)
{
    vector<double> percentageVector;
    percentageVector.reserve(size);

    for (size_t i = 0; i < size; i++)
    {
        percentageVector.push_back(generatePercentage());
    }

    return percentageVector;
}