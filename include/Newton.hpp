#ifndef PEKI_NEWTON_HPP
#define PEKI_NEWTON_HPP

#include "FractalAlgorithm.hpp"

namespace PekiProc {

class Newton : public FractalAlgorithm {
 public:
  CUDA_HD Newton(bool nova = false, bool pixstart = true);
  CUDA_HD Newton(Complex polynomial[], unsigned int polynomial_size,
                 bool nova = false, bool pixstart = true);
  CUDA_HD Newton(Complex polynomial[], unsigned int polynomial_size,
                 const Complex& relaxation, bool nova = false,
                 bool pixstart = true);
  CUDA_HD Newton(Complex polynomial[], unsigned int polynomial_size,
                 const Complex& relaxation, const Complex& incrementation);

  CUDA_HD
  PairGPU<int, TripleGPU<Complex, Complex, Complex>> getIterationsAndOrbit(
      const Complex& c) override;

  int getExponent() override;

  CUDA_HD ~Newton() {}

 private:
  CUDA_HD static Complex computePolynomialValue(Complex polynomial[],
                                                unsigned int size, Complex c);
  CUDA_HD void initPolynomial(Complex polynomial[], unsigned int size);
  CUDA_HD void computeDerivative();

  CUDA_HD Complex f(Complex c);
  CUDA_HD Complex fdx(Complex c);

  Complex m_polynomial[FractalAlgorithmConfiguration::MAX_POLY_TERMS]{};
  Complex m_derivative[FractalAlgorithmConfiguration::MAX_POLY_TERMS]{};
  unsigned int m_polynomial_size = 0;
  Complex relax = Complex::ONE();
  Complex start_value = Complex::ONE();
  bool is_nova = false;
  bool pixel_start = false;
};

}  // namespace PekiProc

#endif  // PEKI_NEWTON_HPP
