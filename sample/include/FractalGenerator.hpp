#ifndef FRACTALGENERATOR_HPP
#define FRACTALGENERATOR_HPP

#include <memory>
#include <vector>
#include "Complex.hpp"
#include "Fractal.hpp"
#include "FractalAlgorithm.hpp"

struct Configuration {
  PekiProc::FractalAlgorithmType fractalType;
  int exponent;
  PekiProc::Complex increment;
  PekiProc::Complex relaxation;
  PekiProc::Complex startValue;
  bool usePixelStart;
  std::vector<PekiProc::Complex> polynomialTerms;
};

class FractalGenerator {
 public:
  void zoom(std::pair<int, int> v1, std::pair<int, int> v2);
  void update(const Configuration& config);

 const uint8_t* imageData();

 private:
  std::unique_ptr<PekiProc::FractalAlgorithm> createAlgorithm();
  std::vector<std::pair<double, PekiProc::RGB>> getDefaultGradient();

  Configuration m_config;
  std::unique_ptr<PekiProc::Fractal> m_fractal;
};

#endif  // FRACTALGENERATOR_HPP
