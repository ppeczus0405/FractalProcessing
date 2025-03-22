#ifndef PEKI_SCALE_HPP
#define PEKI_SCALE_HPP

#include <stdexcept>
#include <string>
#include <utility>

#include "CompareDoubles.hpp"

namespace PekiProc {

class Scale {
 public:
  Scale(int baseW, int baseH, long double widthm, long double widthM,
        long double heightm, long double heightM)
      : baseWidth(baseW),
        baseHeight(baseH),
        widthMin(widthm),
        widthMax(widthM),
        heightMin(heightm),
        heightMax(heightM) {
    if (CompareDoubles::isGreater(widthm, widthM)) {
      throw std::invalid_argument(
          "You can't scale image. Minimal width > Maximal width");
    }  // width_min > width_max
    if (CompareDoubles::isGreater(heightm, heightM)) {
      throw std::invalid_argument(
          "You can't scale image. Minimal height > Maximal height");
    }  // height_min > height_max
    if (baseWidth < 1 || baseHeight < 1) {
      throw std::invalid_argument(
          "One of the base dimensions is less or equal 0");
    }  // Invalid image dimensions

    // Special case handling - when baseWidth/Height = 1;
    if (baseWidth != 1)
      wratio = (widthMax - widthMin) / (baseWidth - 1);
    else
      baseWidthOne = true;

    if (baseHeight != 1)
      hratio = (heightMax - heightMin) / (baseHeight - 1);
    else
      baseHeightOne = true;
  }

  std::pair<long double, long double> getScaled(int x, int y) {
    if (x < 1 || x > baseWidth) {
      std::string error_message =
          "Error: Can't scale pixel. Reason: " + std::to_string(x) +
          "is not in range [1, ";
      error_message.append(std::to_string(baseWidth) + "]");
      throw std::invalid_argument(error_message);
    }
    if (y < 1 || y > baseHeight) {
      std::string error_message =
          "Error: Can't scale pixel. Reason: " + std::to_string(y) +
          "is not in range [1, ";
      error_message.append(std::to_string(baseHeight) + "]");
      throw std::invalid_argument(error_message);
    }

    long double scaledX = (widthMin + widthMax) / 2.0L;
    long double scaledY = (heightMin + heightMax) / 2.0L;
    if (!baseWidthOne)
      scaledX = widthMin + (x - 1) * wratio;
    if (!baseHeightOne)
      scaledY = heightMin + (y - 1) * hratio;
    return {scaledX, scaledY};
  }

  long double getMinReal() { return widthMin; }

  long double getMaxReal() { return widthMax; }

  long double getMinImag() { return heightMin; }

  long double getMaxImag() { return heightMax; }

 private:
  bool baseWidthOne = false, baseHeightOne = false;
  int baseWidth, baseHeight;
  long double widthMin, widthMax, wratio;
  long double heightMin, heightMax, hratio;
};

}  // namespace PekiProc

#endif  // PEKI_SCALE_HPP
