#ifndef PEKI_NEWTON_HPP
#define PEKI_NEWTON_HPP

#include <functional>

#include "FractalAlgorithm.hpp"

namespace PekiProc {

class Newton : public FractalAlgorithm {
 public:
  Newton(bool nova = false, bool pixstart = true);
  Newton(const std::vector<Complex>& polynomial, bool nova = false,
         bool pixstart = true);
  Newton(const std::vector<Complex>& polynomial, const Complex& relaxation,
         bool nova = false, bool pixstart = true);
  Newton(const std::vector<Complex>& polynomial, const Complex& relaxation,
         const Complex& incrementation);

  std::pair<int, std::tuple<Complex, Complex, Complex>> getIterationsAndOrbit(
      const Complex& c) override;
  std::pair<int, std::tuple<Complex, Complex, Complex>>
  getIterationsAndOrbitGPU(const Complex& c) override;
  int getExponent() override;

 private:
  std::function<Complex(Complex)> f = nullptr;
  std::function<Complex(Complex)> fdx = nullptr;

  std::function<Complex(Complex)> createPolynomialFunction(
      const std::vector<Complex>& polynomial);
  void initialize_functions();
  void computeDerivative();

  std::vector<Complex> m_polynomial, m_derivative;
  Complex relax = Complex::ONE;
  Complex start_value = Complex::ONE;
  bool is_nova = false;
  bool pixel_start = false;
};

}  // namespace PekiProc

#endif  // PEKI_NEWTON_HPP
