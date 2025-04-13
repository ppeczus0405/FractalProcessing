#ifndef PEKI_FRACTAL_ALGORITHM_HPP
#define PEKI_FRACTAL_ALGORITHM_HPP

#include <optional>
#include <utility>
#include <vector>

#include "Complex.hpp"
#include "CudaCompat.hpp"
#include "UtilsGPU.hpp"

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
  int maxIterations;

  OptionalGPU<int> exponent;
  OptionalGPU<Complex> increment;
  OptionalGPU<Complex> relaxation;
  OptionalGPU<Complex> startValue;
  OptionalGPU<bool> usePixelStart;
  OptionalGPU<int> polynomialSize;

  // Fixed-size polynomial term buffer for GPU use
  static constexpr int MAX_POLY_TERMS = 16;
  Complex polynomialTerms[MAX_POLY_TERMS];

  CUDA_HD
  FractalAlgorithmConfiguration() {}

  CUDA_HD
  ~FractalAlgorithmConfiguration() {}
};

class FractalAlgorithm {
 public:
  CUDA_HD
  FractalAlgorithm(FractalAlgorithmType falg) : algorithmType(falg) {
    config.maxIterations = max_iter;
  }

  virtual std::pair<int, std::tuple<Complex, Complex, Complex>>
  getIterationsAndOrbit(const Complex& c) = 0;
  virtual int getExponent() = 0;

  CUDA_DEVICE
  virtual PairGPU<int, TripleGPU<Complex, Complex, Complex>>
  getIterationsAndOrbitGPU(const Complex& c) = 0;

  bool setMaxIterationsNumber(int n) {
    // We can only change value if it makes sense
    if (n >= MIN_ITERATIONS && n <= MAX_ITERATIONS) {
      max_iter = n;
      config.maxIterations = max_iter;
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
    os << "Max Iterations: " << config.maxIterations << "\n";
    if (config.exponent.hasValue())
      os << "Exponent: " << config.exponent.get() << "\n";
    if (config.increment.hasValue())
      os << "Increment: " << config.increment.get() << "\n";
    if (config.relaxation.hasValue())
      os << "Relaxation: " << config.relaxation.get() << "\n";
    if (config.startValue.hasValue())
      os << "Start Value: " << config.startValue.get() << "\n";
    if (config.usePixelStart.hasValue())
      os << "Use Pixel Start: "
         << (config.usePixelStart.get() ? "true" : "false") << "\n";
    if (config.polynomialSize.hasValue()) {
      os << "Polynomial size: " << config.polynomialSize.get() << "\n";
      os << "Polynomial Terms: ";
      for (int i = 0; i < config.polynomialSize.get(); i++)
        os << config.polynomialTerms[i] << ", ";
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

  CUDA_HD
  virtual ~FractalAlgorithm() {}

 protected:
  int max_iter = DEFAULT_ITERATIONS;
  FractalAlgorithmConfiguration config;

 private:
  FractalAlgorithmType algorithmType;
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_ALGORITHM_HPP
