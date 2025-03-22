#ifndef PEKI_JULIA_SET_HPP
#define PEKI_JULIA_SET_HPP

#include <utility>

#include "Complex.hpp"
#include "FractalAlgorithm.hpp"

namespace PekiProc {

class JuliaSet : public FractalAlgorithm {
 public:
  JuliaSet();
  JuliaSet(const Complex& inc);
  JuliaSet(const int exponent);
  JuliaSet(const int exponent, const Complex& inc);

  std::pair<int, std::tuple<Complex, Complex, Complex>> getIterationsAndOrbit(
      const Complex& c) override;
  int getExponent() override;

 private:
  Complex m_inc;
  int m_exponent = 2;
  bool m_pixel_as_inc = true;
};

}  // namespace PekiProc

#endif  // PEKI_JULIA_SET_HPP
