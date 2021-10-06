#include <assert.h>
#include "Mandelbrot.hpp"

Mandelbrot::Mandelbrot(const int &exponent) : m_exponent(exponent){
    assert(exponent >= 1);
}

pair<int, tuple<Complex, Complex, Complex> > Mandelbrot::getIterationsAndOrbit(const Complex &c)
{
    int iterations = 0;
    tuple <Complex, Complex, Complex> three_orbit(c, c, c);
    while(iterations < max_iter && CompareDoubles::isLesser(Complex::absolute_square(get<2>(three_orbit)), DIVERGENCE_BAILOUT)){
        get<0>(three_orbit) = get<1>(three_orbit);
        get<1>(three_orbit) = get<2>(three_orbit);
        get<2>(three_orbit) = Complex::power(get<2>(three_orbit), m_exponent) + c;
        iterations++;
    }
    return {iterations, three_orbit};
}