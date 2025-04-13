#include <iostream>
#include "GpuAccelerator.hpp"

namespace PekiProc {

GpuAccelerator::GpuAccelerator(int width, int height, uint8_t* data,
                               const Scale& scale,
                               [[maybe_unused]] FractalAlgorithm* falg,
                               [[maybe_unused]] FractalColoring* fcol) {
  // Allocate and copy width
  cudaMalloc(&m_width, sizeof(int));
  cudaMemcpy(m_width, &width, sizeof(int), cudaMemcpyHostToDevice);

  // Allocate and copy height
  cudaMalloc(&m_height, sizeof(int));
  cudaMemcpy(m_height, &height, sizeof(int), cudaMemcpyHostToDevice);

  // Allocate and copy image data (assumes already initialized on host)
  size_t imageSize = width * height * 3 * sizeof(uint8_t);
  cudaMalloc(&m_data, imageSize);
  cudaMemcpy(m_data, data, imageSize, cudaMemcpyHostToDevice);

  // Allocate and copy Scale object
  cudaMalloc(&m_scale, sizeof(Scale));
  cudaMemcpy(m_scale, &scale, sizeof(Scale), cudaMemcpyHostToDevice);

  // We'll handle falg and fcol next
  m_falg = nullptr;
  m_fcol = nullptr;
}

void GpuAccelerator::generateFractal() {
  std::cout << "To be implemented." << std::endl;
}

GpuAccelerator::~GpuAccelerator() {
  if (m_width)
    cudaFree(m_width);
  if (m_height)
    cudaFree(m_height);
  if (m_data)
    cudaFree(m_data);
  if (m_scale)
    cudaFree(m_scale);

  // m_falg and m_fcol will be handled later when we manage virtual objects on device
  // if (m_falg) cudaFree(m_falg);
  // if (m_fcol) cudaFree(m_fcol);
}

}  // namespace PekiProc
