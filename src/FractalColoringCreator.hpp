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
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent){
            return make_unique<SmoothDivergence>(maxIterations, exponent, DIVERGENCE_BAILOUT);
        }

        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, int mapSize){
            return make_unique<SmoothDivergence>(maxIterations, exponent, DIVERGENCE_BAILOUT, mapSize);
        }
        
        template<typename T>
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, T&& gradient){
            return make_unique<SmoothDivergence>(maxIterations, exponent, DIVERGENCE_BAILOUT, forward<T>(gradient));
        }

        template<typename T>
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent, int mapSize, T&& gradient){
            return make_unique<SmoothDivergence>(maxIterations, exponent, DIVERGENCE_BAILOUT, mapSize, forward<T>(gradient));
        }

        // Convergence
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent){
            return make_unique<SmoothConvergence>(maxIterations, exponent, CONVERGENCE_BAILOUT);
        }

        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, int mapSize){
            return make_unique<SmoothConvergence>(maxIterations, exponent, CONVERGENCE_BAILOUT, mapSize);
        }
        
        template<typename T>
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, T&& gradient){
            return make_unique<SmoothConvergence>(maxIterations, exponent, CONVERGENCE_BAILOUT, forward<T>(gradient));
        }

        template<typename T>
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent, int mapSize, T&& gradient){
            return make_unique<SmoothConvergence>(maxIterations, exponent, CONVERGENCE_BAILOUT, mapSize, forward<T>(gradient));
        }
};

#endif // FRACTAL_COLORING_CREATOR