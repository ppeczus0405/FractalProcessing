#include <assert.h>
#include "Mandelbrot.hpp"

Mandelbrot::Mandelbrot(const int &exponent) : m_exponent(exponent){
    assert(exponent >= 1);
}

pair<int, Complex> Mandelbrot::getIterations(const Complex &c)
{
    int iterations = 0;
    Complex z = c;
    while(iterations < max_iter && CompareDoubles::isLesser(Complex::absolute(z), 2.0L)){
        z = Complex::power(z, m_exponent) + c;
        iterations++;
    }
    return {iterations, z};
}