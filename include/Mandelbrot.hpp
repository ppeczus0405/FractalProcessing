#ifndef PEKI_MANDELBROT_HPP
#define PEKI_MANDELBROT_HPP

#include <utility>

#include "Complex.hpp"
#include "FractalAlgorithm.hpp"

namespace PekiProc {

class Mandelbrot : public FractalAlgorithm {
 public:
  Mandelbrot();
  Mandelbrot(const int exponent);

  std::pair<int, std::tuple<Complex, Complex, Complex>> getIterationsAndOrbit(
      const Complex& c) override;
  std::pair<int, std::tuple<Complex, Complex, Complex>>
  getIterationsAndOrbitGPU(const Complex& c) override;
  int getExponent() override;

 private:
  int m_exponent = 2;
};

}  // namespace PekiProc

#endif  // PEKI_MANDELBROT_HPP
