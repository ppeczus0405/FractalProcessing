#ifndef PEKI_FRACTAL_COLORING_GPU_HPP
#define PEKI_FRACTAL_COLORING_GPU_HPP

#include "Complex.hpp"
#include "CudaCompat.hpp"
#include "SmoothConvergence.hpp"
#include "SmoothDivergence.hpp"
#include "RGB.hpp"

namespace PekiProc {

class FractalColoringGPU {
 public:
  CUDA_DEVICE
  FractalColoringGPU(const FractalColoringConfiguration& cfg)
      : m_bailout(cfg.bailout),
        max_iterations(cfg.maxIterations),
        m_exponent(cfg.exponent),
        map_size(cfg.mapSize),
        color_map(cfg.colorMap) {}

  CUDA_DEVICE virtual RGB getPixel(
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit) = 0;

  CUDA_DEVICE virtual ~FractalColoringGPU() {}

 protected:
  double m_bailout;
  int max_iterations;
  int m_exponent;
  int map_size;
  RGB* color_map;
};

class SmoothConvergenceGPU : public FractalColoringGPU {
 public:
  CUDA_DEVICE
  explicit SmoothConvergenceGPU(const FractalColoringConfiguration& cfg)
      : FractalColoringGPU(cfg) {}

  CUDA_DEVICE
  RGB getPixel(const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit) override {
    return SmoothConvergence::getPixelGeneric(iterOrbit, color_map, map_size,
                                              max_iterations, m_bailout);
  }
};

class SmoothDivergenceGPU : public FractalColoringGPU {
 public:
  CUDA_DEVICE
  explicit SmoothDivergenceGPU(const FractalColoringConfiguration& cfg)
      : FractalColoringGPU(cfg) {}

  CUDA_DEVICE
  RGB getPixel(const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit) override {
    return SmoothDivergence::getPixelGeneric(iterOrbit, color_map, map_size,
                                             max_iterations, m_exponent,
                                             m_bailout);
  }
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_COLORING_GPU_HPP
