#ifndef PEKI_SCALE_HPP
#define PEKI_SCALE_HPP

#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>

#include "CompareDoubles.hpp"
#include "UtilsGPU.hpp"

namespace PekiProc {

class Scale {
 public:
  CUDA_HD Scale(int baseW, int baseH, double widthm, double widthM, double heightm,
        double heightM)
      : baseWidth(baseW),
        baseHeight(baseH),
        widthMin(widthm),
        widthMax(widthM),
        heightMin(heightm),
        heightMax(heightM) {
    if (CompareDoubles::isGreater(widthm, widthM)) {
          assert(!"You can't scale image. Minimal width > Maximal width");
    }  // width_min > width_max
    if (CompareDoubles::isGreater(heightm, heightM)) {
          assert(!"You can't scale image. Minimal height > Maximal height");
    }  // height_min > height_max
    if (baseWidth < 1 || baseHeight < 1) {
          assert(!"One of the base dimensions is less or equal 0");
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

  CUDA_HD PairGPU<double, double> getScaled(int x, int y) {
    if (x < 1 || x > baseWidth) {
      assert(!"getScaled(x, y): x out of range");
    }
    if (y < 1 || y > baseHeight) {
      assert(!"getScaled(x, y): y out of range");
    }

    double scaledX = (widthMin + widthMax) / 2.0;
    double scaledY = (heightMin + heightMax) / 2.0;
    if (!baseWidthOne)
      scaledX = widthMin + (x - 1) * wratio;
    if (!baseHeightOne)
      scaledY = heightMin + (y - 1) * hratio;
    return {scaledX, scaledY};
  }

  CUDA_HD double getMinReal() { return widthMin; }

  CUDA_HD double getMaxReal() { return widthMax; }

  CUDA_HD double getMinImag() { return heightMin; }

  CUDA_HD double getMaxImag() { return heightMax; }

 private:
  bool baseWidthOne = false, baseHeightOne = false;
  int baseWidth, baseHeight;
  double widthMin, widthMax, wratio;
  double heightMin, heightMax, hratio;
};

}  // namespace PekiProc

#endif  // PEKI_SCALE_HPP
