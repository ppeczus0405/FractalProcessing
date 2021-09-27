#include "JuliaSet.hpp"
#include "assert.h"

JuliaSet::JuliaSet(const Complex &inc) : m_inc(inc), m_pixel_as_inc(false) { }

JuliaSet::JuliaSet(const int &exponent) : m_exponent(exponent){
    assert(exponent >= 1);
}

JuliaSet::JuliaSet(const int &exponent, const Complex &inc) : m_inc(inc), m_exponent(exponent), m_pixel_as_inc(false){
    assert(exponent >= 1);
}

pair<int, Complex> JuliaSet::getIterations(const Complex &c) {
    int iterations = 0;
    Complex z = c;
    while(iterations < max_iter && CompareDoubles::isLesser(Complex::absolute(z), 2.0L)){
        z = Complex::power(z, m_exponent);
        z += m_pixel_as_inc ? c : m_inc;
        iterations++;
    }
    return {iterations, z};
}