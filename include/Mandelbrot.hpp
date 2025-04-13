#ifndef PEKI_MANDELBROT_HPP
#define PEKI_MANDELBROT_HPP

#include <utility>

#include "Complex.hpp"
#include "FractalAlgorithm.hpp"

namespace PekiProc {

class Mandelbrot : public FractalAlgorithm {
 public:
  CUDA_HD Mandelbrot();
  CUDA_HD Mandelbrot(const int exponent);

  std::pair<int, std::tuple<Complex, Complex, Complex>> getIterationsAndOrbit(
      const Complex& c) override;

  CUDA_DEVICE
  PairGPU<int, TripleGPU<Complex, Complex, Complex>> getIterationsAndOrbitGPU(
      const Complex& c) override;

  int getExponent() override;

  CUDA_HD ~Mandelbrot() {}

 private:
  int m_exponent = 2;
};

}  // namespace PekiProc

#endif  // PEKI_MANDELBROT_HPP
