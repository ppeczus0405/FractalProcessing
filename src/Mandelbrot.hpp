#ifndef MANDELBROT_HEADER
#define MANDELBROT_HEADER

#include "Complex.hpp"

class Mandelbrot
{
    public:
        static int getIterations(Complex p);
        static int getMaxIterations();
        static void setMaxIterations(int iterNumber);
    private:
        static int MAX_ITERATIONS;
        Mandelbrot();
};

#endif // MANDELBROT HEADER