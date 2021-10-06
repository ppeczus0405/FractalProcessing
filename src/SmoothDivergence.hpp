#ifndef SMOOTH_DIVERGENCE
#define SMOOTH_DIVERGENCE

#include "FractalColoring.hpp"
#include "CompareDoubles.hpp"

class SmoothDivergence : public FractalColoring{
    public:
        SmoothDivergence(int maxIterations, int exponent, long double bailout) : 
            FractalColoring(maxIterations, exponent, bailout) { }

        SmoothDivergence(int maxIterations, int exponent, long double bailout, int mapSize) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize) { }

        SmoothDivergence(int maxIterations, int exponent, long double bailout, const Gradient& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, gradient) { }

        SmoothDivergence(int maxIterations, int exponent, long double bailout, Gradient&& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, move(gradient)) { }

        SmoothDivergence(int maxIterations, int exponent, long double bailout, int mapSize, const Gradient& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize, gradient) { }

        SmoothDivergence(int maxIterations, int exponent, long double bailout, int mapSize, Gradient&& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize, move(gradient)) { }

        virtual RGB getPixel(const pair <int, tuple <Complex, Complex, Complex> > &iterOrbit) override
        {
            auto iterations = iterOrbit.first;
            auto threeOrbit = iterOrbit.second;
            long double R = Complex::absolute_square(get<1>(threeOrbit));
            
            // Special case when reached max iterations number or logarithm is undefined
            if(iterations == max_iterations || CompareDoubles::isEqual(R, 1.0L)){
                return color_map[map_size - 1];
            }

            // Smooth factor
            long double smooth = log2(log2(m_bailout) / log2(R)) / log2(m_exponent);
            smooth = max(0.0L, min(1.0L, smooth));

            long double ratio = (map_size - 1) / (long double)(max_iterations);
            long double value = ratio * iterations;
            long double prev_value = ratio * (iterations - 1);
            int index = smooth * value + (1.0L - smooth) * prev_value;
            index = max(0, min(map_size - 1, index));
            return color_map[index];
        }
};


#endif // SMOOTH_DIVERGENCE