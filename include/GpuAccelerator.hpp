#ifndef PEKI_GPU_ACCELERATOR_HPP
#define PEKI_GPU_ACCELERATOR_HPP

#include <cstdint>
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"
#include "FractalColoringGPU.hpp"
#include "Scale.hpp"

namespace PekiProc {

// Value of members below points to location that is accessible from GPU kernel function.
struct KernelProcessingData {
  int* width;
  int* height;
  uint8_t* image_data;
  Scale* scale;
  FractalAlgorithm** falg;
  FractalColoringGPU** fcol;
  RGB* color_map;
  int* map_size;
};

class GpuAccelerator {
 public:
  GpuAccelerator() = default;
  GpuAccelerator(int width, int height, uint8_t* data, const Scale& scale,
                 FractalAlgorithm* falg, FractalColoring* fcol);
  void generateFractal();

  static KernelProcessingData* kernel_data;

  ~GpuAccelerator();

 private:
  KernelProcessingData host_data{};
  FractalAlgorithmConfiguration* d_algorithmConfiguration;
};

}  // namespace PekiProc

#endif  // PEKI_GPU_ACCELERATOR_HPP
