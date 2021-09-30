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
        Mandelbrot() = default;
        Mandelbrot(const int &exponent); // For Multibrot
        pair<int, tuple<Complex, Complex, Complex> > getIterations(const Complex &c) override;
    private:
        // Default value for Mandelbrot set, values other than 2 means Multibrot set
        int m_exponent = 2; 
};

#endif // MANDELBROT HEADER