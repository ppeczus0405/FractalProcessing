#ifndef PEKI_GRADIENT_HPP
#define PEKI_GRADIENT_HPP

#include "ImageSaver.hpp"
#include "RGB.hpp"
#include "spline.hpp"

namespace PekiProc {

enum class InterpolationTarget : uint8_t { RED, GREEN, BLUE };

class Gradient {
 public:
  Gradient();
  Gradient(const double x, const RGB& color);
  Gradient(const std::vector<std::pair<double, RGB>>& gradient_points);

  static const Gradient WHITE_BLACK;

  bool insertPoint(const double x, const RGB& color);
  bool changePoint(const double x, const RGB& color);
  bool isPointInside(const double x) noexcept;
  bool removePoint(const double x);
  bool write(std::string filename, int w = ImageSaver::DEFAULT_SAVE_WIDTH,
             int h = ImageSaver::DEFAULT_SAVE_HEIGHT);

  RGB getPointColor(const double x);
  std::vector<std::pair<double, RGB>> getPoints() noexcept;

  std::vector<RGB> generateGradientMap(int map_size);

 private:
  std::vector<std::pair<double, RGB>> grad_points;
  std::vector<std::tuple<tk::spline, int, int>> mono_intervals[RGB::RGB_SIZE];
  bool interpolated = false;

  std::vector<std::pair<double, RGB>>::iterator getPointIter(
      const double x) noexcept;
  void generateMonotoneIntervals();
  RGB interpolate(const double x);
};

}  // namespace PekiProc

#endif  // PEKI_GRADIENT_HPP
