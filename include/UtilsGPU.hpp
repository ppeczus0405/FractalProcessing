#ifndef PEKI_UTILS_GPU_HPP
#define PEKI_UTILS_GPU_HPP

#include "CudaCompat.hpp"

namespace PekiProc {

template <typename T>
struct OptionalGPU {
  bool has_value = false;
  T value{};

  CUDA_HD OptionalGPU() { }

  CUDA_HD OptionalGPU(T val) : has_value(true), value(val) {}

  CUDA_HD bool hasValue() const { return has_value; }

  CUDA_HD T get() const { return value; }

  CUDA_HD ~OptionalGPU() {}
};

}  // namespace PekiProc

template <typename T1, typename T2>
struct PairGPU {
  T1 first;
  T2 second;

  CUDA_HD PairGPU() { }

  CUDA_HD PairGPU(T1 a, T2 b) : first(a), second(b) {}

  CUDA_HD ~PairGPU() { }
};

template <typename T1, typename T2, typename T3>
struct TripleGPU {
  T1 first;
  T2 second;
  T3 third;

  CUDA_HD TripleGPU() { }

  CUDA_HD TripleGPU(T1 a, T2 b, T3 c) : first(a), second(b), third(c) {}

  CUDA_HD ~TripleGPU() { }
};

#endif // PEKI_UTILS_GPU_HPP
