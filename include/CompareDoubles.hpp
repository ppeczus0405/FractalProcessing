#ifndef PEKI_COMPARE_DOUBLES_HPP
#define PEKI_COMPARE_DOUBLES_HPP

#include <cmath>

namespace PekiProc {

class CompareDoubles {
 public:
  CompareDoubles() = delete;

  constexpr static double EPSILON_LONG = 1e-10;
  constexpr static double EPSILON = 1e-7;

  static bool isEqual(const double a, const double b) {
    return fabs(a - b) < EPSILON;
  }

  static bool isGreater(const double a, const double b) {
    return a - b >= EPSILON;
  }

  static bool isLesser(const double a, const double b) {
    return a - b <= -EPSILON;
  }
};

}  // namespace PekiProc

#endif  // PEKI_COMPARE_DOUBLES_HPP
