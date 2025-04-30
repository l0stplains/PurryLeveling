#ifndef RNG_HPP
#define RNG_HPP

#include <vector>
using namespace std;

#define usl unsigned long long


class RNG {
private:
    usl state;

    const usl multiplier = static_cast<usl>(6364136223846793005);
    const usl increment = static_cast<usl>(1442695040888963407);
    
public:
    RNG(); // constructor (default: 69420)
    RNG(usl seed); // constructor (user defined)
    ~RNG(); // destructor
    
    unsigned int generateNext(); // next random 32 bit
    unsigned int generateInRange(unsigned int max); // Generate a random number [0, max-1]
    unsigned int generateInRange(unsigned int min, unsigned int max); // Generate a random number [min, max]
    
    double generateProbability();
    double generatePercentage();
    
    void setSeed(usl seed); // set seed val
    
    vector<unsigned int> generateRandomVector(size_t size, unsigned int max);
    vector<double> generatePercentageVector(size_t size);
};

#endif
