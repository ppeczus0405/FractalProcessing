#ifndef SMOOTH_CONVERGENCE
#define SMOOTH_CONVERGENCE

#include "FractalColoring.hpp"

class SmoothConvergence : public FractalColoring{
    public:
        SmoothConvergence(int maxIterations, int exponent, long double bailout) : 
            FractalColoring(maxIterations, exponent, bailout) { }

        SmoothConvergence(int maxIterations, int exponent, long double bailout, int mapSize) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize) { }

        SmoothConvergence(int maxIterations, int exponent, long double bailout, const Gradient& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, gradient) { }

        SmoothConvergence(int maxIterations, int exponent, long double bailout, Gradient&& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, move(gradient)) { }

        SmoothConvergence(int maxIterations, int exponent, long double bailout, int mapSize, const Gradient& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize, gradient) { }

        SmoothConvergence(int maxIterations, int exponent, long double bailout, int mapSize, Gradient&& gradient) : 
            FractalColoring(maxIterations, exponent, bailout, mapSize, move(gradient)) { }

        virtual RGB getPixel(const pair <int, tuple <Complex, Complex, Complex> > &iterOrbit) override
        {
            auto iterations = iterOrbit.first;
            auto threeOrbit = iterOrbit.second;
            auto z  = get<2>(threeOrbit);
            auto z1 = get<1>(threeOrbit);
            auto z2 = get<0>(threeOrbit);

            // Special case when reached max iterations number or something goes wrong(e.g dx(complex) = 0 in Newton method)
            if(iterations == max_iterations || iterations == -1){
                return color_map[map_size - 1];
            }

            long double R      = Complex::absolute_square(z - z1)  / max(1.0L, Complex::absolute_square(z));
            long double prev_R = Complex::absolute_square(z1 - z2) / max(1.0L, Complex::absolute_square(z1));
            
            // Smooth factor
            long double smooth = (log2(m_bailout) - log2(prev_R)) / (log2(R) - log2(prev_R));
            smooth = max(0.0L, min(1.0L, smooth));

            long double ratio = (map_size - 1) / (long double)(max_iterations);
            long double value = ratio * iterations;
            long double prev_value = ratio * (iterations - 1);
            int index = smooth * value + (1.0L - smooth) * prev_value;
            index = max(0, min(map_size - 1, index));
            return color_map[index];
        }
};


#endif // SMOOTH_CONVERGENCE

