#ifndef PEKI_FRACTAL_COLORING_CREATOR_HPP
#define PEKI_FRACTAL_COLORING_CREATOR_HPP

#include "FractalColoring.hpp"
#include "SmoothConvergence.hpp"
#include "SmoothDivergence.hpp"

namespace PekiProc {

class FractalColoringCreator {
 public:
  FractalColoringCreator() = delete;

  static std::unique_ptr<FractalColoring> createSmoothDivergence(
      int maxIterations, int exponent,
      int mapSize = FractalColoring::DEFAULT_COLOR_MAP_SIZE,
      std::unique_ptr<Gradient> gradient = nullptr) {
    return std::make_unique<SmoothDivergence>(maxIterations, exponent, mapSize,
                                              std::move(gradient));
  }

  static std::unique_ptr<FractalColoring> createSmoothConvergence(
      int maxIterations, int exponent,
      int mapSize = FractalColoring::DEFAULT_COLOR_MAP_SIZE,
      std::unique_ptr<Gradient> gradient = nullptr) {
    return std::make_unique<SmoothConvergence>(maxIterations, exponent, mapSize,
                                               std::move(gradient));
  }
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_COLORING_CREATOR_HPP
