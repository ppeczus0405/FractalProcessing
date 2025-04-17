#ifndef PEKI_SMOOTH_CONVERGENCE_HPP
#define PEKI_SMOOTH_CONVERGENCE_HPP

#include <math.h>
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"

namespace PekiProc {

class SmoothConvergence : public FractalColoring {
 public:
  SmoothConvergence(int maxIterations, int exponent,
                    int mapSize = DEFAULT_COLOR_MAP_SIZE,
                    std::unique_ptr<Gradient> gradient = nullptr)
      : FractalColoring(maxIterations, exponent,
                        FractalAlgorithm::CONVERGENCE_BAILOUT, mapSize,
                        std::move(gradient)) {}

  virtual RGB getPixel(const PairGPU<int, TripleGPU<Complex, Complex, Complex>>&
                           iterOrbit) override {
    return SmoothConvergence::getPixelGeneric(
        iterOrbit, color_map.data(), map_size, max_iterations, m_bailout);
  }

 public:
  CUDA_HD
  static RGB getPixelGeneric(
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit,
      const RGB* colorMap, int mapSize, int maxIterations, double bailout) {
    const int iterations = iterOrbit.first;
    const auto& threeOrbit = iterOrbit.second;
    const Complex& z = threeOrbit.first;
    const Complex& z1 = threeOrbit.second;
    const Complex& z2 = threeOrbit.third;

    // Escaped or invalid
    if (iterations == maxIterations || iterations == -1) {
      return colorMap[mapSize - 1];
    }

    const double R = Complex::absolute_square(z - z1) /
                     ::fmax(1.0, Complex::absolute_square(z));
    const double prev_R = Complex::absolute_square(z1 - z2) /
                          ::fmax(1.0, Complex::absolute_square(z1));

    const double denom = ::log2(R) - ::log2(prev_R);
    double smooth = 0.0;
    if (denom != 0.0) {
      smooth = (::log2(bailout) - ::log2(prev_R)) / denom;
      smooth = clamp(smooth, 0.0, 1.0);
    }

    const double ratio = (mapSize - 1) / static_cast<double>(maxIterations);
    const double value = ratio * iterations;
    const double prev_value = ratio * (iterations - 1);
    int index = static_cast<int>(smooth * value + (1.0 - smooth) * prev_value);
    index = clamp(index, 0, mapSize - 1);

    return colorMap[index];
  }
};

}  // namespace PekiProc

#endif  // PEKI_SMOOTH_CONVERGENCE_HPP
