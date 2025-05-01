#ifndef RNG_HPP
#define RNG_HPP

#include <vector>
using namespace std;

/**
 * @brief Type definition for unsigned long long to simplify code
 */
#define usl unsigned long long

/**
 * @brief Random Number Generator class
 *
 * This class provides functionality for generating random numbers using
 * a Linear Congruential Generator (LCG) algorithm. It can generate single
 * random numbers, probabilities, percentages, and vectors of random values.
 */
class RNG
{
private:
    usl state;  ///< Current state of the random number generator

    /** @brief Multiplier constant for the LCG algorithm */
    const usl multiplier = static_cast<usl>(6364136223846793005);

    /** @brief Increment constant for the LCG algorithm */
    const usl increment = static_cast<usl>(1442695040888963407);

public:
    /**
     * @brief Default constructor
     *
     * Initializes the random number generator with a default seed of 69420
     */
    RNG();

    /**
     * @brief Constructor with user-defined seed
     *
     * @param seed The seed value to initialize the random number generator
     */
    RNG(usl seed);

    /**
     * @brief Destructor
     */
    ~RNG();

    /**
     * @brief Assignment operator overloading
     *
     * Copies the state of another RNG object to this one
     *
     * @param other The RNG object to copy from
     * @return Reference to this RNG object
     */
    RNG& operator=(const RNG& other);

    /**
     * @brief Generate the next random 32-bit unsigned integer
     *
     * @return An unsigned 32-bit random integer
     */
    unsigned int generateNext();

    /**
     * @brief Generate a random number in range [0, max-1]
     *
     * @param max The exclusive upper bound of the random range
     * @return A random number between 0 and max-1 (inclusive)
     */
    unsigned int generateInRange(unsigned int max);

    /**
     * @brief Generate a random number in range [min, max]
     *
     * @param min The inclusive lower bound of the random range
     * @param max The inclusive upper bound of the random range
     * @return A random number between min and max (inclusive)
     */
    unsigned int generateInRange(unsigned int min, unsigned int max);

    /**
     * @brief Generate a random probability value between 0.0 and 1.0
     *
     * @return A random double value between 0.0 and 1.0
     */
    double generateProbability();

    /**
     * @brief Generate a random percentage value between 0.0 and 100.0
     *
     * @return A random double value between 0.0 and 100.0
     */
    double generatePercentage();

    /**
     * @brief Set the seed value for the random number generator
     *
     * @param seed The seed value to set
     */
    void setSeed(usl seed);

    /**
     * @brief Generate a vector of random unsigned integers
     *
     * @param size The number of random values to generate
     * @param max The exclusive upper bound for the random values
     * @return A vector containing the specified number of random values
     */
    vector<unsigned int> generateRandomVector(size_t size, unsigned int max);

    /**
     * @brief Generate a vector of random percentage values
     *
     * @param size The number of percentage values to generate
     * @return A vector containing the specified number of random percentages
     */
    vector<double> generatePercentageVector(size_t size);
};

#endif