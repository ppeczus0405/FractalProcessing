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
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit)
      override {
    auto iterations = iterOrbit.first;
    auto threeOrbit = iterOrbit.second;
    auto z = threeOrbit.first;
    auto z1 = threeOrbit.second;
    auto z2 = threeOrbit.third;

    // Special case when reached max iterations number or something goes
    // wrong(e.g dx(complex) = 0 in Newton method)
    if (iterations == max_iterations || iterations == -1) {
      return color_map[map_size - 1];
    }

    double R = Complex::absolute_square(z - z1) /
               std::max(1.0, Complex::absolute_square(z));
    double prev_R = Complex::absolute_square(z1 - z2) /
                    std::max(1.0, Complex::absolute_square(z1));

    // Smooth factor
    double smooth = (log2(m_bailout) - log2(prev_R)) / (log2(R) - log2(prev_R));
    smooth = std::max(0.0, std::min(1.0, smooth));

    double ratio = (map_size - 1) / (double)(max_iterations);
    double value = ratio * iterations;
    double prev_value = ratio * (iterations - 1);
    int index = smooth * value + (1.0 - smooth) * prev_value;
    index = std::max(0, std::min(map_size - 1, index));
    return color_map[index];
  }
};

}  // namespace PekiProc

#endif  // PEKI_SMOOTH_CONVERGENCE_HPP
