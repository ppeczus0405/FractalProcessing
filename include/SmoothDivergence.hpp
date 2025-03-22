#ifndef PEKI_SMOOTH_DIVERGENCE_HPP
#define PEKI_SMOOTH_DIVERGENCE_HPP

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
                        std::move(gradient)) {}

  virtual RGB getPixel(
      const std::pair<int, std::tuple<Complex, Complex, Complex>>& iterOrbit)
      override {
    auto iterations = iterOrbit.first;
    auto threeOrbit = iterOrbit.second;
    long double R = Complex::absolute_square(get<1>(threeOrbit));

    // Special case when reached max iterations number or logarithm is undefined
    if (iterations == max_iterations || CompareDoubles::isEqual(R, 1.0L)) {
      return color_map[map_size - 1];
    }

    // Smooth factor
    long double smooth = log2(log2(m_bailout) / log2(R)) / log2(m_exponent);
    smooth = std::max(0.0L, std::min(1.0L, smooth));

    long double ratio = (map_size - 1) / (long double)(max_iterations);
    long double value = ratio * iterations;
    long double prev_value = ratio * (iterations - 1);
    int index = smooth * value + (1.0L - smooth) * prev_value;
    index = std::max(0, std::min(map_size - 1, index));
    return color_map[index];
  }
};

}  // namespace PekiProc

#endif  // PEKI_SMOOTH_DIVERGENCE_HPP
