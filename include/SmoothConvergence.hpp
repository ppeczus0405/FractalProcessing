#ifndef PEKI_SMOOTH_CONVERGENCE_HPP
#define PEKI_SMOOTH_CONVERGENCE_HPP

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

  virtual RGB getPixel(
      const std::pair<int, std::tuple<Complex, Complex, Complex>>& iterOrbit)
      override {
    auto iterations = iterOrbit.first;
    auto threeOrbit = iterOrbit.second;
    auto z = std::get<2>(threeOrbit);
    auto z1 = std::get<1>(threeOrbit);
    auto z2 = std::get<0>(threeOrbit);

    // Special case when reached max iterations number or something goes
    // wrong(e.g dx(complex) = 0 in Newton method)
    if (iterations == max_iterations || iterations == -1) {
      return color_map[map_size - 1];
    }

    long double R = Complex::absolute_square(z - z1) /
                    std::max(1.0L, Complex::absolute_square(z));
    long double prev_R = Complex::absolute_square(z1 - z2) /
                         std::max(1.0L, Complex::absolute_square(z1));

    // Smooth factor
    long double smooth =
        (log2(m_bailout) - log2(prev_R)) / (log2(R) - log2(prev_R));
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

#endif  // PEKI_SMOOTH_CONVERGENCE_HPP
