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

std::pair<int, std::tuple<Complex, Complex, Complex>>
JuliaSet::getIterationsAndOrbit(const Complex& c) {
  int iterations = 0;
  std::tuple<Complex, Complex, Complex> three_orbit(c, c, c);
  while (iterations < max_iter &&
         CompareDoubles::isLesser(
             Complex::absolute_square(std::get<2>(three_orbit)),
             DIVERGENCE_BAILOUT)) {
    std::get<0>(three_orbit) = std::get<1>(three_orbit);
    std::get<1>(three_orbit) = std::get<2>(three_orbit);
    std::get<2>(three_orbit) =
        Complex::power(std::get<2>(three_orbit), m_exponent) +
        (m_pixel_as_inc ? c : m_inc);
    iterations++;
  }
  return {iterations, three_orbit};
}

CUDA_DEVICE
PairGPU<int, TripleGPU<Complex, Complex, Complex>>
JuliaSet::getIterationsAndOrbitGPU([[maybe_unused]] const Complex& c) {
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
