#include <cassert>

#include "CompareDoubles.hpp"
#include "JuliaSet.hpp"

namespace PekiProc {

using FAT = FractalAlgorithmType;

CUDA_HD
JuliaSet::JuliaSet() : FractalAlgorithm(FAT::JULIA) {}

CUDA_HD
JuliaSet::JuliaSet(const Complex& inc)
    : FractalAlgorithm(FAT::JULIA), m_inc(inc), m_pixel_as_inc(false) {
  config.increment = m_inc;
}

CUDA_HD
JuliaSet::JuliaSet(const int exponent)
    : FractalAlgorithm(FAT::POLYJULIA), m_exponent(exponent) {
  assert(exponent >= 1);
  config.exponent = m_exponent;
}

CUDA_HD
JuliaSet::JuliaSet(const int exponent, const Complex& inc)
    : FractalAlgorithm(FAT::POLYJULIA),
      m_inc(inc),
      m_exponent(exponent),
      m_pixel_as_inc(false) {
  config.exponent = m_exponent;
  config.increment = m_inc;
  assert(exponent >= 1);
}

CUDA_HD
PairGPU<int, TripleGPU<Complex, Complex, Complex>>
JuliaSet::getIterationsAndOrbit(const Complex& c) {
  int iterations = 0;
  TripleGPU<Complex, Complex, Complex> three_orbit(c, c, c);
  while (iterations < max_iter &&
         CompareDoubles::isLesser(Complex::absolute_square(three_orbit.third),
                                  DIVERGENCE_BAILOUT)) {
    three_orbit.first = three_orbit.second;
    three_orbit.second = three_orbit.third;
    three_orbit.third = Complex::power(three_orbit.third, m_exponent) +
                        (m_pixel_as_inc ? c : m_inc);
    iterations++;
  }
  return {iterations, three_orbit};
}

int JuliaSet::getExponent() {
  return m_exponent;
}

}  // namespace PekiProc
