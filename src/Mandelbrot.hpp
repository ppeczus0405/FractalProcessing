#ifndef MANDELBROT_HEADER
#define MANDELBROT_HEADER

#include "Complex.hpp"
#include <utility>
using namespace std;

class Mandelbrot
{
    public:
        static pair<Complex, int> getIterations(Complex p);
        static int getMaxIterations();
        static void setMaxIterations(int iterNumber);
    private:
        static int MAX_ITERATIONS;
        Mandelbrot();
};

#endif // MANDELBROT HEADER