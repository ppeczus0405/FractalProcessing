#ifndef PEKI_GPU_ACCELERATOR_HPP
#define PEKI_GPU_ACCELERATOR_HPP

#include <cstdint>
#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"
#include "FractalColoringGPU.hpp"
#include "Scale.hpp"

namespace PekiProc {

namespace kernel {

CUDA_KERNEL void createFractalInterfacesOnDevice(
    FractalAlgorithm** algoPtr, FractalAlgorithmConfiguration* config,
    FractalColoringGPU** coloringPtr);

CUDA_KERNEL void deinitFractalInterfacesOnDevice(
    FractalAlgorithm** algoPtr, FractalColoringGPU** coloringPtr);

}  // namespace kernel

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

  friend CUDA_KERNEL void kernel::createFractalInterfacesOnDevice(
      FractalAlgorithm** algoPtr, FractalAlgorithmConfiguration* config,
      FractalColoringGPU** coloringPtr);

 private:
  CUDA_DEVICE static void fractalAlgorithmDeviceInit(
      FractalAlgorithm** falg, FractalAlgorithmConfiguration* config);
  CUDA_DEVICE static void fractalColoringDeviceInit(FractalColoring** fcol);

  void initializeVirtualInterfacesOnDevice();
  void deinitializeVirtualInterfacesOnDevice();

 private:
  KernelProcessingData host_data{};
  FractalAlgorithmConfiguration* d_algorithmConfiguration{nullptr};
};

}  // namespace PekiProc

#endif  // PEKI_GPU_ACCELERATOR_HPP
