#include "FractalGenerator.hpp"
#include "FractalAlgorithmCreator.hpp"

std::unique_ptr<PekiProc::FractalAlgorithm>
FractalGenerator::createAlgorithm() {
  using namespace PekiProc;

  switch (m_config.fractalType) {
    case FractalAlgorithmType::MANDELBROT:
      return FractalAlgorithmCreator::createMandelbrot();

    case FractalAlgorithmType::MULTIBROT:
      return FractalAlgorithmCreator::createMultibrot(m_config.exponent);

    case FractalAlgorithmType::JULIA:
      return FractalAlgorithmCreator::createJuliaSet(m_config.increment);

    case FractalAlgorithmType::POLYJULIA:
      return FractalAlgorithmCreator::createPolynomialJuliaSet(
          m_config.exponent, m_config.increment);

    case FractalAlgorithmType::NEWTON:
      return FractalAlgorithmCreator::createNewton(m_config.polynomialTerms,
                                                   m_config.relaxation);

    case FractalAlgorithmType::NOVA:
      if (m_config.usePixelStart)
        return FractalAlgorithmCreator::createNova(m_config.polynomialTerms,
                                                   m_config.relaxation, true);
      else
        return FractalAlgorithmCreator::createNova(
            m_config.polynomialTerms, m_config.relaxation, m_config.startValue);

    default:
      throw std::invalid_argument("Unsupported fractal algorithm type.");
  }
}

std::vector<std::pair<double, PekiProc::RGB>> FractalGenerator::getDefaultGradient() {
  return {
      {0.0, {0, 7, 100}},       // Deep Navy Blue
      {0.16, {32, 107, 203}},   // Vivid Blue
      {0.33, {237, 255, 255}},  // Pale Cyan
      {0.5, {255, 170, 0}},     // Bright Orange
      {0.67, {255, 85, 0}},     // Vivid Red-Orange
      {0.84, {128, 0, 128}},    // Purple
      {1.0, {0, 0, 0}}          // Black
  };
}

void FractalGenerator::update(const Configuration& config) {
  m_config = config;

  m_fractal = PekiProc::FractalBuilder(1080, 720)
                  .setMaxIterations(30)
                  .setGradient(getDefaultGradient())
                  .setGradientMapSize(1024)
                  .setAlgorithm(createAlgorithm())
                  .build();
}

const uint8_t* FractalGenerator::imageData() {
  return m_fractal->getData();
}

void FractalGenerator::zoom([[maybe_unused]] std::pair<int, int> v1,
                            [[maybe_unused]] std::pair<int, int> v2) {}
