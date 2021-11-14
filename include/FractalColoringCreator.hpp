#ifndef FRACTAL_COLORING_CREATOR
#define FRACTAL_COLORING_CREATOR
 
#include "FractalColoring.hpp"
#include "SmoothDivergence.hpp"
#include "SmoothConvergence.hpp"
 
class FractalColoringCreator
{
    public:
        FractalColoringCreator() = delete;
 
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent,
                                                                  int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
        {
            return make_unique<SmoothDivergence>(maxIterations, exponent, mapSize, move(gradient));
        }
 
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent,
                                                                  int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
        {
            return make_unique<SmoothConvergence>(maxIterations, exponent, mapSize, move(gradient));
        }
};
 
#endif // FRACTAL_COLORING_CREATOR