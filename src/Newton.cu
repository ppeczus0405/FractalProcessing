#include "CompareDoubles.hpp"
#include "Newton.hpp"

namespace PekiProc {

using FAT = FractalAlgorithmType;

// Newton constructors
CUDA_HD Newton::Newton(bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      is_nova(nova),
      pixel_start(pixstart) {
  // z^3 - 1 = 0
  m_polynomial_size = 4;
  m_polynomial[0] = -Complex::ONE();
  m_polynomial[1] = Complex::ZERO();
  m_polynomial[2] = Complex::ZERO();
  m_polynomial[3] = Complex::ONE();

  config.usePixelStart = pixel_start;
}

CUDA_HD Newton::Newton(Complex polynomial[], unsigned int polynomial_size,
                       bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      is_nova(nova),
      pixel_start(pixstart) {
  initPolynomial(polynomial, polynomial_size);
  config.usePixelStart = pixel_start;
}

CUDA_HD Newton::Newton(Complex polynomial[], unsigned int polynomial_size,
                       const Complex& relaxation, bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      relax(relaxation),
      is_nova(nova),
      pixel_start(pixstart) {
  initPolynomial(polynomial, polynomial_size);
  config.relaxation = relax;
  config.usePixelStart = pixel_start;
}

CUDA_HD Newton::Newton(Complex polynomial[], unsigned int polynomial_size,
                       const Complex& relaxation, const Complex& startval)
    : FractalAlgorithm(FAT::NOVA),
      relax(relaxation),
      start_value(startval),
      is_nova(true),
      pixel_start(false) {
  initPolynomial(polynomial, polynomial_size);
  config.relaxation = relax;
  config.startValue = start_value;
}

CUDA_HD void Newton::initPolynomial(Complex polynomial[], unsigned int size) {
  for (int i = 0; i < size; i++)
    m_polynomial[i] = config.polynomialTerms[i] = polynomial[i];
  config.polynomialSize = m_polynomial_size = size;
  // Compute derivative
  int exponent = 1;
  for (int i = 1; i < m_polynomial_size; i++) {
    m_derivative[i - 1] = m_polynomial[i] * exponent++;
  }
}

CUDA_HD Complex Newton::computePolynomialValue(Complex polynomial[],
                                               unsigned int size, Complex c) {
  Complex ans = Complex::ZERO();
  Complex z = Complex::ONE();
  for (unsigned int i = 0; i < size; i++) {
    ans += z * polynomial[i];
    z *= c;
  }
  return ans;
}

CUDA_HD Complex Newton::f(Complex c) {
  return computePolynomialValue(m_polynomial, m_polynomial_size, c);
}

CUDA_HD Complex Newton::fdx(Complex c) {
  return computePolynomialValue(
      m_derivative, m_polynomial_size > 0 ? m_polynomial_size - 1 : 0, c);
}

std::pair<int, std::tuple<Complex, Complex, Complex>>
Newton::getIterationsAndOrbit(const Complex& c) {
  auto nextIter = [&](Complex z) {
    Complex dx_value = fdx(z);
    if (dx_value == Complex::ZERO())
      return std::make_pair(false, Complex::ZERO());
    return std::make_pair(
        true, z - relax * (f(z) / dx_value) + (is_nova ? c : Complex::ZERO()));
  };
  auto checkEndPoint = [](Complex a, Complex b) {
    return CompareDoubles::isGreater(
        Complex::absolute_square(b - a) /
            std::max(1.0, Complex::absolute_square(b)),
        CONVERGENCE_BAILOUT);
  };

  int iters = 0;
  bool notEnd = true;
  Complex z0 = pixel_start ? c : start_value;
  std::tuple<Complex, Complex, Complex> three_orbit(z0, z0, z0);

  while (notEnd && iters < max_iter) {
    auto next = nextIter(std::get<2>(three_orbit));
    // Encounter case when derivative is equal to zero
    if (!next.first)
      return make_pair(-1, three_orbit);
    std::get<0>(three_orbit) = std::get<1>(three_orbit);
    std::get<1>(three_orbit) = std::get<2>(three_orbit);
    std::get<2>(three_orbit) = next.second;
    notEnd = checkEndPoint(std::get<1>(three_orbit), std::get<2>(three_orbit));
    iters++;
  }
  return {iters, three_orbit};
}

CUDA_DEVICE
PairGPU<int, TripleGPU<Complex, Complex, Complex>>
Newton::getIterationsAndOrbitGPU([[maybe_unused]] const Complex& c) {
  int iters = 0;
  bool notEnd = true;

  Complex z0 = pixel_start ? c : start_value;
  Complex z1 = z0;
  Complex z2 = z0;

  while (notEnd && iters < max_iter) {
    Complex dx_value = fdx(z2);
    if (dx_value == Complex::ZERO()) {
      return {-1, {z0, z1, z2}};
    }

    Complex fz = f(z2);
    Complex delta = fz / dx_value;
    Complex next = z2 - relax * delta;
    if (is_nova) {
      next = next + c;
    }

    // Rotate the orbit
    z0 = z1;
    z1 = z2;
    z2 = next;

    // Check convergence
    Complex diff = z2 - z1;
    double num = Complex::absolute_square(diff);
    double denom = max(1.0, Complex::absolute_square(z2));
    notEnd = CompareDoubles::isGreater(num / denom, CONVERGENCE_BAILOUT);

    iters++;
  }
  return {iters, {z0, z1, z2}};
}

int Newton::getExponent() {
  return m_polynomial_size - 1;
}

}  // namespace PekiProc
