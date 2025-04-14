#include <iostream>
#include "FractalAlgorithm.hpp"
#include "GpuAccelerator.hpp"
#include "Mandelbrot.hpp"

namespace PekiProc {

namespace kernel {
CUDA_KERNEL void createFractalInterfacesOnDevice(
    FractalAlgorithm** algoPtr, FractalAlgorithmConfiguration* config,
    FractalColoringGPU** coloringPtr) {
  if (threadIdx.x == 0 && blockIdx.x == 0) {
    GpuAccelerator::fractalAlgorithmDeviceInit(algoPtr, config);
  }
}

CUDA_KERNEL void deinitFractalInterfacesOnDevice(
    FractalAlgorithm** algoPtr, FractalColoringGPU** coloringPtr) {
  if (threadIdx.x == 0 && blockIdx.x == 0) {
    if (*algoPtr != nullptr) {
      printf("[DEVICE] Not null algorithm pointer. Deleting!\n");
      delete *algoPtr;
    }
    if (*coloringPtr != nullptr) {
      printf("[DEVICE] Not null coloring pointer. Deleting!\n");
      delete *coloringPtr;
    }
  }
}

}  // namespace kernel

KernelProcessingData* GpuAccelerator::kernel_data = nullptr;

GpuAccelerator::GpuAccelerator(int width, int height, uint8_t* data,
                               const Scale& scale, FractalAlgorithm* falg,
                               FractalColoring* fcol) {
  // --- Allocate all necessary device buffers ---
  int* d_width = nullptr;
  int* d_height = nullptr;
  uint8_t* d_image = nullptr;
  Scale* d_scale = nullptr;
  FractalAlgorithm** d_falg = nullptr;
  FractalColoringGPU** d_fcol = nullptr;
  RGB* d_color_map = nullptr;
  int* d_map_size = nullptr;

  // Width and height
  cudaMalloc(&d_width, sizeof(int));
  cudaMemcpy(d_width, &width, sizeof(int), cudaMemcpyHostToDevice);

  cudaMalloc(&d_height, sizeof(int));
  cudaMemcpy(d_height, &height, sizeof(int), cudaMemcpyHostToDevice);

  // Image data
  size_t imageSize = width * height * 3 * sizeof(uint8_t);
  cudaMalloc(&d_image, imageSize);
  cudaMemcpy(d_image, data, imageSize, cudaMemcpyHostToDevice);

  // Scale
  cudaMalloc(&d_scale, sizeof(Scale));
  cudaMemcpy(d_scale, &scale, sizeof(Scale), cudaMemcpyHostToDevice);

  // FractalAlgorithm** pointer
  cudaMalloc(
      &d_falg,
      sizeof(FractalAlgorithm*));  // object will be created later on device
  FractalAlgorithm* falgNullPtr = nullptr;
  cudaMemcpy(d_falg, &falgNullPtr, sizeof(FractalAlgorithm*),
             cudaMemcpyHostToDevice);

  // Assume falg is already initialized and contains configuration
  const FractalAlgorithmConfiguration& h_config =
      falg->getFractalAlgorithmConfig();

  // Allocate on device
  cudaMalloc(&d_algorithmConfiguration, sizeof(FractalAlgorithmConfiguration));
  cudaMemcpy(d_algorithmConfiguration, &h_config,
             sizeof(FractalAlgorithmConfiguration), cudaMemcpyHostToDevice);

  // FractalColoringGPU**
  cudaMalloc(
      &d_fcol,
      sizeof(
          FractalColoringGPU*));  // object will also be created later on device
  FractalColoringGPU* fcolNullPtr = nullptr;
  cudaMemcpy(d_fcol, &fcolNullPtr, sizeof(FractalColoringGPU*),
             cudaMemcpyHostToDevice);

  // Color map
  int map_size = fcol->getColorMapSize();
  const RGB* color_map_host = fcol->getColorMap();

  cudaMalloc(&d_map_size, sizeof(int));
  cudaMemcpy(d_map_size, &map_size, sizeof(int), cudaMemcpyHostToDevice);

  cudaMalloc(&d_color_map, sizeof(RGB) * map_size);
  cudaMemcpy(d_color_map, color_map_host, sizeof(RGB) * map_size,
             cudaMemcpyHostToDevice);

  // --- Fill host-side struct ---
  host_data = {.width = d_width,
               .height = d_height,
               .image_data = d_image,
               .scale = d_scale,
               .falg = d_falg,
               .fcol = d_fcol,
               .color_map = d_color_map,
               .map_size = d_map_size};

  // --- Initialize virtual interfaces ---
  initializeVirtualInterfacesOnDevice();

  // --- Copy full struct to device ---
  cudaMalloc(&kernel_data, sizeof(KernelProcessingData));
  cudaMemcpy(kernel_data, &host_data, sizeof(KernelProcessingData),
             cudaMemcpyHostToDevice);
}

CUDA_DEVICE void GpuAccelerator::fractalAlgorithmDeviceInit(
    FractalAlgorithm** falg, FractalAlgorithmConfiguration* config) {
  switch (config->fractalType) {
    case PekiProc::FractalAlgorithmType::MANDELBROT:
      printf("[DEVICE] Mandelbrot fractal creation\n");
      if (config->exponent.hasValue()) {
        *falg = new Mandelbrot(config->exponent.get());
      } else {
        *falg = new Mandelbrot();
      }
      break;
    default:
      printf("[DEVICE] Default fractal creation\n");
  }
}

CUDA_DEVICE void GpuAccelerator::fractalColoringDeviceInit(
    FractalColoring** fcol) {}

void GpuAccelerator::initializeVirtualInterfacesOnDevice() {
  kernel::createFractalInterfacesOnDevice<<<1, 1>>>(
      host_data.falg, d_algorithmConfiguration, host_data.fcol);

  cudaDeviceSynchronize();
}

void GpuAccelerator::generateFractal() {
  std::cout << "Not implemented yet." << std::endl;
}

void GpuAccelerator::deinitializeVirtualInterfacesOnDevice() {
  kernel::deinitFractalInterfacesOnDevice<<<1, 1>>>(host_data.falg,
                                                    host_data.fcol);
  cudaDeviceSynchronize();
}

GpuAccelerator::~GpuAccelerator() {
  if (host_data.width)
    cudaFree(host_data.width);
  if (host_data.height)
    cudaFree(host_data.height);
  if (host_data.image_data)
    cudaFree(host_data.image_data);
  if (host_data.scale)
    cudaFree(host_data.scale);
  if (host_data.color_map)
    cudaFree(host_data.color_map);
  if (host_data.map_size)
    cudaFree(host_data.map_size);

  // Virtual interfaces
  deinitializeVirtualInterfacesOnDevice();
  if (host_data.fcol)
    cudaFree(host_data.fcol);
  if (host_data.falg)
    cudaFree(host_data.falg);

  // Kernel Data
  if (kernel_data) {
    cudaFree(kernel_data);
    kernel_data = nullptr;
  }
}

}  // namespace PekiProc
