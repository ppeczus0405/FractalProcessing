#ifndef MANDELBROT_HEADER
#define MANDELBROT_HEADER

#include "Complex.hpp"
#include "CompareDoubles.hpp"
#include "FractalAlgorithm.hpp"
#include <utility>
using namespace std;

class Mandelbrot : public FractalAlgorithm
{
    public:
        Mandelbrot();
        Mandelbrot(const int exponent);
        
        pair<int, tuple<Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) override;
        int getExponent() override;

    private:
        int m_exponent = 2; 
};

#endif // MANDELBROT HEADER