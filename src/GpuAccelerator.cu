#include <iostream>
#include "GpuAccelerator.hpp"

namespace PekiProc {

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

  // FractalColoringGPU**
  cudaMalloc(
      &d_fcol,
      sizeof(
          FractalColoringGPU*));  // object will also be created later on device

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

  // --- Copy full struct to device ---
  cudaMalloc(&kernel_data, sizeof(KernelProcessingData));
  cudaMemcpy(kernel_data, &host_data, sizeof(KernelProcessingData),
             cudaMemcpyHostToDevice);
}

void GpuAccelerator::generateFractal() {
  std::cout << "Not implemented yet." << std::endl;
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
  if (host_data.fcol)
    cudaFree(host_data.fcol);
  if (host_data.falg)
    cudaFree(host_data.falg);
  if (kernel_data) {
    cudaFree(kernel_data);
    kernel_data = nullptr;
  }
}

}  // namespace PekiProc
