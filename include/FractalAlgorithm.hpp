#ifndef PEKI_FRACTAL_ALGORITHM_HPP
#define PEKI_FRACTAL_ALGORITHM_HPP

#include <utility>

#include "Complex.hpp"

namespace PekiProc {

enum class FractalAlgorithmType {
  MANDELBROT,
  MULTIBROT,
  JULIA,
  POLYJULIA,
  NEWTON,
  NOVA
};

class FractalAlgorithm {
 public:
  FractalAlgorithm(FractalAlgorithmType falg) : algorithmType(falg) {}

  virtual std::pair<int, std::tuple<Complex, Complex, Complex>>
  getIterationsAndOrbit(const Complex& c) = 0;
  virtual int getExponent() = 0;

  bool setMaxIterationsNumber(int n) {
    // We can only change value if it makes sense
    if (n >= MIN_ITERATIONS && n <= MAX_ITERATIONS) {
      max_iter = n;
      return true;
    } else {
      std::cerr << "Iteration value have to be integer meeting the condition: ";
      std::cerr << MIN_ITERATIONS << " <= iterations <= " << MAX_ITERATIONS
                << std::endl;
      std::cerr << "Not changed. Value = " << max_iter << std::endl;
      return false;
    }
  }

  FractalAlgorithmType getAlgorithmType() { return algorithmType; }

  int getMaxIterationsNumber() const { return max_iter; }

  static constexpr int MAX_ITERATIONS = 1e4;
  static constexpr int MIN_ITERATIONS = 1;
  static constexpr int DEFAULT_ITERATIONS = 35;

  static constexpr double DIVERGENCE_BAILOUT = 1e8;
  static constexpr double CONVERGENCE_BAILOUT = 1e-14;

  virtual ~FractalAlgorithm() = default;

 protected:
  int max_iter = DEFAULT_ITERATIONS;

 private:
  FractalAlgorithmType algorithmType;
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_ALGORITHM_HPP
