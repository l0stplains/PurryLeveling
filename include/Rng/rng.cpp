#include "rng.hpp"

/*
    to my felllow friends, i have a simple note for you:
    1. to get single random number, you must create the object first, then do generateNext()

    example:
    RNG rng();
    double num = rng.generatePercentage();

    or

    RNG rng(69420);
    double num = rng.generatePercentage();

    2. to get multiple random numbers, you can use generatePercentageVector() or
   generateRandomVector() example: RNG rng(69420); vector<double> num =
   rng.generatePercentageVector(10); 10 is the number of random numbers

    3. to compare, make a boolean var:
    example:
    RNG rng(69420);
    double num = rng.generatePercentage();
    bool target = num <= 50.0;

    if (target) {
        cout << "success" << endl;
    } else {
        cout << "fail" << endl;
    }
*/

RNG::RNG()
{
    setSeed(69420);
}

RNG::RNG(usl seed)
{
    setSeed(seed);
}

void RNG::setSeed(usl seed)
{
    this->state = 0;
    generateNext();
    state += seed;
    generateNext();
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
