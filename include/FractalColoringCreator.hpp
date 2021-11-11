#ifndef FRACTAL_COLORING_CREATOR
#define FRACTAL_COLORING_CREATOR

#include "FractalColoring.hpp"
#include "SmoothDivergence.hpp"
#include "SmoothConvergence.hpp"

/**
 * @brief Set of factory functions for creating fractal colorings
 * 
 */
class FractalColoringCreator
{
    public:
        /**
         * @brief Disable possibility creating #FractalColoringCreator objects
         * 
         */
        FractalColoringCreator() = delete;

        /**
         * @brief Factory function for #SmoothDivergence instantiation
         * 
         * @param maxIterations - maximal number of iterations that fractal algorithm can perform
         * @param exponent - highest power of variable in polynomial used to generate fractal image
         * @param mapSize - size of the gradient color map
         * @param gradient - object used to color fractal image
         * @return pointer to the new instance of #SmoothDivergence class
         */
        static unique_ptr<FractalColoring> createSmoothDivergence(int maxIterations, int exponent,
                                                                  int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
        {
            return make_unique<SmoothDivergence>(maxIterations, exponent, mapSize, move(gradient));
        }

        /**
         * @brief Factory function for #SmoothConvergence instantiation
         * 
         * @param maxIterations - maximal number of iterations that fractal algorithm can perform
         * @param exponent - highest power of variable in polynomial used to generate fractal image
         * @param mapSize - size of the gradient color map
         * @param gradient - object used to color fractal image
         * @return pointer to the new instance of #SmoothConvergence class
         */
        static unique_ptr<FractalColoring> createSmoothConvergence(int maxIterations, int exponent,
                                                                  int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
        {
            return make_unique<SmoothConvergence>(maxIterations, exponent, mapSize, move(gradient));
        }
};

#endif // FRACTAL_COLORING_CREATOR