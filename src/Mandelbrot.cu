#include <cassert>

#include "CompareDoubles.hpp"
#include "Mandelbrot.hpp"

namespace PekiProc {

using FAT = FractalAlgorithmType;

CUDA_HD
Mandelbrot::Mandelbrot() : FractalAlgorithm(FAT::MANDELBROT) {}

CUDA_HD
Mandelbrot::Mandelbrot(const int exponent)
    : FractalAlgorithm(FAT::MULTIBROT), m_exponent(exponent) {
  assert(exponent >= 1);
  config.exponent = m_exponent;
}

std::pair<int, std::tuple<Complex, Complex, Complex>>
Mandelbrot::getIterationsAndOrbit(const Complex& c) {
  int iterations = 0;
  std::tuple<Complex, Complex, Complex> three_orbit(c, c, c);
  while (iterations < max_iter &&
         CompareDoubles::isLesser(
             Complex::absolute_square(std::get<2>(three_orbit)),
             DIVERGENCE_BAILOUT)) {
    std::get<0>(three_orbit) = std::get<1>(three_orbit);
    std::get<1>(three_orbit) = std::get<2>(three_orbit);
    std::get<2>(three_orbit) =
        Complex::power(std::get<2>(three_orbit), m_exponent) + c;
    iterations++;
  }
  return {iterations, three_orbit};
}

CUDA_DEVICE
PairGPU<int, TripleGPU<Complex, Complex, Complex>>
Mandelbrot::getIterationsAndOrbitGPU(const Complex& c) {
  int iterations = 0;
  TripleGPU<Complex, Complex, Complex> three_orbit(c, c, c);
  while (iterations < max_iter &&
         CompareDoubles::isLesser(Complex::absolute_square(three_orbit.third),
                                  DIVERGENCE_BAILOUT)) {
    three_orbit.first = three_orbit.second;
    three_orbit.second = three_orbit.third;
    three_orbit.third = Complex::power(three_orbit.third, m_exponent) + c;
    iterations++;
  }
  return {iterations, three_orbit};
}

int Mandelbrot::getExponent() {
  return m_exponent;
}

}  // namespace PekiProc
