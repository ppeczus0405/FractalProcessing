#ifndef PEKI_COMPARE_DOUBLES_HPP
#define PEKI_COMPARE_DOUBLES_HPP

#include <cmath>
#include "CudaCompat.hpp"

namespace PekiProc {

class CompareDoubles {
 public:
  CompareDoubles() = delete;

  constexpr static double EPSILON = 1e-10;

  CUDA_HD
  static bool isEqual(const double a, const double b) {
    return fabs(a - b) < EPSILON;
  }

  CUDA_HD
  static bool isGreater(const double a, const double b) {
    return a - b >= EPSILON;
  }

  CUDA_HD
  static bool isLesser(const double a, const double b) {
    return a - b <= -EPSILON;
  }
};

}  // namespace PekiProc

#endif  // PEKI_COMPARE_DOUBLES_HPP
