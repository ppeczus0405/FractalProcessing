#ifndef PEKI_GPU_ACCELERATOR_HPP
#define PEKI_GPU_ACCELERATOR_HPP

#include <cstdint>
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"
#include "Scale.hpp"

namespace PekiProc {

class GpuAccelerator {
 public:
  GpuAccelerator() = default;
  GpuAccelerator(int width, int height, uint8_t* data, const Scale& scale,
                 FractalAlgorithm* falg, FractalColoring* fcol);
  void generateFractal();
  ~GpuAccelerator();

 private:
  // Value of members below points to location that is accessible from GPU kernel function.
  int* m_width{nullptr};
  int* m_height{nullptr};
  uint8_t* m_data{nullptr};
  Scale* m_scale{nullptr};
  FractalAlgorithm* m_falg{nullptr};
  FractalColoring* m_fcol{nullptr};
};

}  // namespace PekiProc

#endif  // PEKI_GPU_ACCELERATOR_HPP
