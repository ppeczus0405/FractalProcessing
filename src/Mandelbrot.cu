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

CUDA_HD
PairGPU<int, TripleGPU<Complex, Complex, Complex>>
Mandelbrot::getIterationsAndOrbit(const Complex& c) {
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
