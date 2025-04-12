#include <cuda_runtime.h>
#include <iostream>

const int N = 1 << 20;  // 1M elements (4MB if int)

// CUDA kernel to initialize array on device
__global__ void init_array(int* arr, int value) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (idx < N) {
    arr[idx] = value;
  }
}

// Host-side validation
void validate(int* arr, int expected) {
  for (int i = 0; i < N; i++) {
    if (arr[i] != expected) {
      std::cerr << "Validation failed at index " << i << ": " << arr[i]
                << std::endl;
      return;
    }
  }
  std::cout << "✅ Validation passed! All values are " << expected << "."
            << std::endl;
}

int main() {
  int* data;

  // Allocate Unified Memory
  cudaError_t err = cudaMallocManaged(&data, N * sizeof(int));
  if (err != cudaSuccess) {
    std::cerr << "cudaMallocManaged failed: " << cudaGetErrorString(err)
              << std::endl;
    return 1;
  }

  // Prefetch to GPU (Optional, requires compute capability >= 6.0)
  int device = -1;
  cudaGetDevice(&device);
  cudaMemPrefetchAsync(data, N * sizeof(int), device, nullptr);

  // Launch kernel
  int threads = 256;
  int blocks = (N + threads - 1) / threads;
  init_array<<<blocks, threads>>>(data, 42);
  cudaDeviceSynchronize();

  // Prefetch back to CPU
  cudaMemPrefetchAsync(data, N * sizeof(int), cudaCpuDeviceId, nullptr);
  cudaDeviceSynchronize();

  // Validate on CPU
  validate(data, 42);

  // Free memory
  cudaFree(data);
  return 0;
}
