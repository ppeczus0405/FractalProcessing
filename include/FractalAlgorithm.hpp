#ifndef PEKI_FRACTAL_ALGORITHM_HPP
#define PEKI_FRACTAL_ALGORITHM_HPP

#include <utility>
#include <optional>
#include <vector>

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

struct FractalAlgorithmConfiguration {
  FractalAlgorithmType fractalType = FractalAlgorithmType::MANDELBROT;
  int max_iterations;
  std::optional<int> exponent;
  std::optional<Complex> increment;
  std::optional<Complex> relaxation;
  std::optional<Complex> startValue;
  std::optional<bool> usePixelStart;
  std::optional<std::vector<Complex>> polynomialTerms;
};

class FractalAlgorithm {
 public:
  FractalAlgorithm(FractalAlgorithmType falg) : algorithmType(falg) {
   config.max_iterations = max_iter;
  }

  virtual std::pair<int, std::tuple<Complex, Complex, Complex>>
  getIterationsAndOrbit(const Complex& c) = 0;
  virtual int getExponent() = 0;

  bool setMaxIterationsNumber(int n) {
    // We can only change value if it makes sense
    if (n >= MIN_ITERATIONS && n <= MAX_ITERATIONS) {
      max_iter = n;
      config.max_iterations = max_iter;
      return true;
    } else {
      std::cerr << "Iteration value have to be integer meeting the condition: ";
      std::cerr << MIN_ITERATIONS << " <= iterations <= " << MAX_ITERATIONS
                << std::endl;
      std::cerr << "Not changed. Value = " << max_iter << std::endl;
      return false;
    }
  }
  
  virtual void dumpConfig(std::ostream& os = std::cout) {
    os << "Fractal Type: " << static_cast<int>(config.fractalType) << "\n";
    os << "Max Iterations: " << config.max_iterations << "\n";
    if (config.exponent) os << "Exponent: " << *config.exponent << "\n";
    if (config.increment) os << "Increment: " << *config.increment << "\n";
    if (config.relaxation) os << "Relaxation: " << *config.relaxation << "\n";
    if (config.startValue) os << "Start Value: " << *config.startValue << "\n";
    if (config.usePixelStart) os << "Use Pixel Start: " << (*config.usePixelStart ? "true" : "false") << "\n";
    if (config.polynomialTerms) {
      os << "Polynomial Terms: ";
      for (const auto& term : *config.polynomialTerms)
        os << term << ", ";
      os << "\n";
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
  FractalAlgorithmConfiguration config;

 private:
  FractalAlgorithmType algorithmType;
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_ALGORITHM_HPP
