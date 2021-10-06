#ifndef FRACTAL_COLORING_CREATOR
#define FRACTAL_COLORING_CREATOR

#include "FractalColoring.hpp"
#include "SmoothDivergence.hpp"
#include "SmoothConvergence.hpp"

class FractalColoringCreator
{
    public:
        FractalColoringCreator() = delete;

        // Divergence 
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, long double bailout){
            return make_unique<SmoothDivergence>(maxIterations, exponent, bailout);
        }

        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, long double bailout, int mapSize){
            return make_unique<SmoothDivergence>(maxIterations, exponent, bailout, mapSize);
        }
        
        template<typename T>
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, long double bailout, T&& gradient){
            return make_unique<SmoothDivergence>(maxIterations, exponent, bailout, forward<T>(gradient));
        }

        template<typename T>
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, long double bailout, int mapSize, T&& gradient){
            return make_unique<SmoothDivergence>(maxIterations, exponent, bailout, mapSize, forward<T>(gradient));
        }

        // Convergence
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, long double bailout){
            return make_unique<SmoothConvergence>(maxIterations, exponent, bailout);
        }

        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, long double bailout, int mapSize){
            return make_unique<SmoothConvergence>(maxIterations, exponent, bailout, mapSize);
        }
        
        template<typename T>
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, long double bailout, T&& gradient){
            return make_unique<SmoothConvergence>(maxIterations, exponent, bailout, forward<T>(gradient));
        }

        template<typename T>
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, long double bailout, int mapSize, T&& gradient){
            return make_unique<SmoothConvergence>(maxIterations, exponent, bailout, mapSize, forward<T>(gradient));
        }
};

#endif // FRACTAL_COLORING_CREATOR