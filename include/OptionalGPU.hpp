#ifndef PEKI_OPTIONAL_GPU_HPP
#define PEKI_OPTIONAL_GPU_HPP

#include "CudaCompat.hpp"

namespace PekiProc {

template <typename T>
struct OptionalGPU {
  bool has_value = false;
  T value{};

  CUDA_HOST CUDA_DEVICE
  OptionalGPU() { }

  CUDA_HOST CUDA_DEVICE
  OptionalGPU(T val) : has_value(true), value(val) {}

  CUDA_HOST CUDA_DEVICE
  bool hasValue() const { return has_value; }

  CUDA_HOST CUDA_DEVICE
  T get() const { return value; }
};

}  // namespace PekiProc

#endif // PEKI_OPTIONAL_GPU_HPP
