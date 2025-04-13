#ifndef PEKI_FRACTAL_COLORING_GPU_HPP
#define PEKI_FRACTAL_COLORING_GPU_HPP

#include "Complex.hpp"
#include "CudaCompat.hpp"
#include "RGB.hpp"

namespace PekiProc {

class FractalColoringGPU {
 public:
  CUDA_DEVICE
  FractalColoringGPU(int maxIterations, int exponent, double bailout,
                     int mapSize, RGB* color_map)
      : m_bailout(bailout),
        max_iterations(maxIterations),
        m_exponent(exponent),
        map_size(mapSize),
        color_map(color_map) {}

  CUDA_DEVICE
  virtual RGB getPixel(
      const std::pair<int, std::tuple<Complex, Complex, Complex>>&
          iterOrbit) = 0;

  CUDA_DEVICE
  virtual ~FractalColoringGPU() {};

 protected:
  double m_bailout;

  int max_iterations;
  int m_exponent;
  int map_size = -1;

  RGB* color_map;
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_COLORING_GPU_HPP
