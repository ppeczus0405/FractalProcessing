#ifndef PEKI_SMOOTH_DIVERGENCE_HPP
#define PEKI_SMOOTH_DIVERGENCE_HPP

#include "CompareDoubles.hpp"
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"
#include <math.h>

namespace PekiProc {

class SmoothDivergence : public FractalColoring {
 public:
  SmoothDivergence(int maxIterations, int exponent,
                   int mapSize = DEFAULT_COLOR_MAP_SIZE,
                   std::unique_ptr<Gradient> gradient = nullptr)
      : FractalColoring(maxIterations, exponent,
                        FractalAlgorithm::DIVERGENCE_BAILOUT, mapSize,
                        std::move(gradient)) {}

  virtual RGB getPixel(
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit)
      override {
    return SmoothDivergence::getPixelGeneric(iterOrbit, color_map.data(),
                                             map_size, max_iterations,
                                             m_exponent, m_bailout);
  }

 public:
  CUDA_HD
  static RGB getPixelGeneric(
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit,
      const RGB* colorMap, int mapSize, int maxIterations, int exponent,
      double bailout) {
    const int iterations = iterOrbit.first;
    const auto& threeOrbit = iterOrbit.second;
    const double R = Complex::absolute_square(threeOrbit.second);

    // Special case – reached max iterations or undefined logarithm
    if (iterations == maxIterations || CompareDoubles::isEqual(R, 1.0)) {
      return colorMap[mapSize - 1];
    }

    // Smooth factor
    const double smooth = clamp(
        ::log2(::log2(bailout) / ::log2(R)) /
            ::log2(static_cast<double>(exponent)),
        0.0, 1.0);

    const double ratio = (mapSize - 1) / static_cast<double>(maxIterations);
    const double value = ratio * iterations;
    const double prev_value = ratio * (iterations - 1);
    int index =
        static_cast<int>(smooth * value + (1.0 - smooth) * prev_value);
    index = clamp(index, 0, mapSize - 1);
    return colorMap[index];
  }
};

}  // namespace PekiProc

#endif  // PEKI_SMOOTH_DIVERGENCE_HPP

