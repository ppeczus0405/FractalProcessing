#ifndef PEKI_SMOOTH_DIVERGENCE_HPP
#define PEKI_SMOOTH_DIVERGENCE_HPP

#include <math.h>
#include "CompareDoubles.hpp"
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"

namespace PekiProc {

class SmoothDivergence : public FractalColoring {
 public:
  SmoothDivergence(int maxIterations, int exponent,
                   int mapSize = DEFAULT_COLOR_MAP_SIZE,
                   std::unique_ptr<Gradient> gradient = nullptr)
      : FractalColoring(maxIterations, exponent,
                        FractalAlgorithm::DIVERGENCE_BAILOUT, mapSize,
                        std::move(gradient)) {
    color_config.coloringType = FractalColoringType::SMOOTH_DIVERGENCE;
  }

  virtual RGB getPixel(const PairGPU<int, TripleGPU<Complex, Complex, Complex>>&
                           iterOrbit) override {
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
    return colorMap[iterations];
  }
};

}  // namespace PekiProc

#endif  // PEKI_SMOOTH_DIVERGENCE_HPP
