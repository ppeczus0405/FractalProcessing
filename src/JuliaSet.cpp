#include "JuliaSet.hpp"
#include "assert.h"

JuliaSet::JuliaSet(const Complex &inc) : m_inc(inc), m_pixel_as_inc(false) { }

JuliaSet::JuliaSet(const int &exponent) : m_exponent(exponent){
    assert(exponent >= 1);
}

JuliaSet::JuliaSet(const int &exponent, const Complex &inc) : m_inc(inc), m_exponent(exponent), m_pixel_as_inc(false){
    assert(exponent >= 1);
}

pair<int, tuple<Complex, Complex, Complex> > JuliaSet::getIterations(const Complex &c) {
    int iterations = 0;
    tuple <Complex, Complex, Complex> three_orbit(c, c, c);
    while(iterations < max_iter && CompareDoubles::isLesser(Complex::absolute_square(get<2>(three_orbit)), DIVERGENCE_BAILOUT)){
        get<0>(three_orbit) = get<1>(three_orbit);
        get<1>(three_orbit) = get<2>(three_orbit);
        get<2>(three_orbit) = Complex::power(get<2>(three_orbit), m_exponent) + (m_pixel_as_inc ? c : m_inc);
        iterations++;
    }
    return {iterations, three_orbit};
}