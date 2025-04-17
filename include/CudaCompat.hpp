#ifndef PEKI_CUDA_COMPAT_HPP
#define PEKI_CUDA_COMPAT_HPP

#ifdef __CUDACC__
#define CUDA_HOST __host__
#define CUDA_DEVICE __device__
#define CUDA_HD __host__ __device__
#define CUDA_KERNEL __global__
#define CUDA_COMPATIBLE 1
#else
#define CUDA_HOST
#define CUDA_DEVICE
#define CUDA_HD
#define CUDA_KERNEL
#define CUDA_COMPATIBLE 0
#endif

#endif  // PEKI_CUDA_COMPAT_HPP
