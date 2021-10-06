#ifndef FRACTAL_ALGORITHM
#define FRACTAL_ALGORITHM

#include "Complex.hpp"
#include <memory>
#include <utility>
using namespace std;

// Max iterations are limited due to performance
constexpr int MAX_ITERATIONS = 1e4;
constexpr int MIN_ITERATIONS = 1;
constexpr int DEFAULT_ITERATIONS = 250;

// Bailouts
constexpr long double DIVERGENCE_BAILOUT = 1e8;
constexpr long double CONVERGENCE_BAILOUT = 1e-14;

class FractalAlgorithm
{
    public:
        FractalAlgorithm() = default;
        
        // Function to get number of iterations and end point using fractal algorithm
        virtual pair<int, tuple<Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) = 0;
        
        inline void setMaxIterationsNumber(int n){
            // We can only change value if it makes sense
            if(n >= MIN_ITERATIONS && n <= MAX_ITERATIONS){
                max_iter = n;
            }
        }

        inline int getMaxIterationsNumber() const{
            return max_iter;
        }

    protected:
        int max_iter = DEFAULT_ITERATIONS;
};

#endif // FRACTAL_ALGORITHM