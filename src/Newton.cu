#include <ranges>

#include "Newton.hpp"

namespace PekiProc {

using FAT = FractalAlgorithmType;

// Newton constructors
Newton::Newton(bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      is_nova(nova),
      pixel_start(pixstart) {
  // z^3 - 1 = 0
  m_polynomial = {Complex::ONE, Complex::ZERO, Complex::ZERO, -Complex::ONE};
  initialize_functions();
}

Newton::Newton(const std::vector<Complex>& polynomial, bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      m_polynomial(polynomial),
      is_nova(nova),
      pixel_start(pixstart) {
  initialize_functions();
}

Newton::Newton(const std::vector<Complex>& polynomial,
               const Complex& relaxation, bool nova, bool pixstart)
    : FractalAlgorithm(nova ? FAT::NOVA : FAT::NEWTON),
      m_polynomial(polynomial),
      relax(relaxation),
      is_nova(nova),
      pixel_start(pixstart) {
  initialize_functions();
}

Newton::Newton(const std::vector<Complex>& polynomial,
               const Complex& relaxation, const Complex& startval)
    : FractalAlgorithm(FAT::NOVA),
      m_polynomial(polynomial),
      relax(relaxation),
      start_value(startval),
      is_nova(true),
      pixel_start(false) {
  initialize_functions();
}

void Newton::initialize_functions() {
  computeDerivative();
  f = createPolynomialFunction(m_polynomial);
  fdx = createPolynomialFunction(m_derivative);
}

void Newton::computeDerivative() {
  m_derivative = m_polynomial;
  if ((int)m_polynomial.size() ==
      0)  // empty polynomial is treated as W(c) = 0, where c is complex number
    return;
  m_derivative.pop_back();
  int exponent = 1;
  for (auto it = m_derivative.rbegin(); it != m_derivative.rend(); it++)
    *it *= exponent++;
}

std::function<Complex(Complex)> Newton::createPolynomialFunction(
    const std::vector<Complex>& polynomial) {
  std::function<Complex(Complex)> func = [&polynomial](Complex c) {
    Complex ans = Complex::ZERO;
    Complex z = Complex::ONE;
    for (auto it = polynomial.rbegin(); it != polynomial.rend(); it++) {
      ans += z * (*it);
      z *= c;
    }
    return ans;
  };
  return func;
}

std::pair<int, std::tuple<Complex, Complex, Complex>>
Newton::getIterationsAndOrbit(const Complex& c) {
  auto nextIter = [&](Complex z) {
    Complex dx_value = fdx(z);
    if (dx_value == Complex::ZERO)
      return std::make_pair(false, Complex::ZERO);
    return std::make_pair(
        true, z - relax * (f(z) / dx_value) + (is_nova ? c : Complex::ZERO));
  };
  auto checkEndPoint = [](Complex a, Complex b) {
    return Complex::absolute_square(b - a) /
               std::max(1.0, Complex::absolute_square(b)) <=
           CONVERGENCE_BAILOUT;
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
    notEnd = !checkEndPoint(std::get<1>(three_orbit), std::get<2>(three_orbit));
    iters++;
  }
  return {iters, three_orbit};
}

std::pair<int, std::tuple<Complex, Complex, Complex>>
Newton::getIterationsAndOrbitGPU([[maybe_unused]] const Complex& c) {
  return {};
}

int Newton::getExponent() {
  return (int)m_polynomial.size() - 1;
}

}  // namespace PekiProc
