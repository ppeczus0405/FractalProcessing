#ifndef PEKI_FRACTAL_COLORING_HPP
#define PEKI_FRACTAL_COLORING_HPP

#include "Complex.hpp"
#include "Gradient.hpp"
#include "RGB.hpp"

namespace PekiProc {

class FractalColoring {
 public:
  FractalColoring(int maxIterations, int exponent, double bailout,
                  int mapSize = DEFAULT_COLOR_MAP_SIZE,
                  std::unique_ptr<Gradient> gradient = nullptr)
      : FractalColoring(exponent, bailout) {
    if (gradient)
      m_gradient = std::move(gradient);
    else
      m_gradient = std::make_unique<Gradient>(Gradient::WHITE_BLACK);

    setColorMapSize(mapSize);
    max_iterations = maxIterations;
  }

  void setGradient(std::unique_ptr<Gradient> gradient) {
    if (gradient) {
      m_gradient = std::move(gradient);
      color_map = m_gradient->generateGradientMap(map_size);
    }
  }

  std::unique_ptr<Gradient> getGradient() noexcept {
    return std::move(m_gradient);
  }

  int getColorMapSize() const noexcept { return map_size; }

  bool setColorMapSize(int mapSize) {
    if (mapSize >= MIN_MAP_SIZE && mapSize <= MAX_MAP_SIZE) {
      if (map_size != mapSize) {
        map_size = mapSize;
        color_map = m_gradient->generateGradientMap(map_size);
        return true;
      }
      return false;
    } else {
      std::cerr << "Map size has to meet the condition: ";
      std::cerr << MIN_MAP_SIZE << " <= size <= " << MAX_MAP_SIZE << std::endl;
      if (map_size == -1) {
        std::cerr << "Setted map size to " << DEFAULT_COLOR_MAP_SIZE
                  << std::endl;
        map_size = DEFAULT_COLOR_MAP_SIZE;
        color_map = m_gradient->generateGradientMap(map_size);
        return true;
      } else {
        std::cerr << "Size not changed" << std::endl;
        return false;
      }
    }
  }

  void setMaxIterations(int iters) { max_iterations = iters; }

  static constexpr int DEFAULT_COLOR_MAP_SIZE = 512;
  static constexpr int MAX_MAP_SIZE = 16384;
  static constexpr int MIN_MAP_SIZE = 1;

  virtual RGB getPixel(
      const std::pair<int, std::tuple<Complex, Complex, Complex>>&
          iterOrbit) = 0;

  virtual ~FractalColoring() = default;

 protected:
  double m_bailout;

  int max_iterations;
  int m_exponent;
  int map_size = -1;

  std::unique_ptr<Gradient> m_gradient = nullptr;
  std::vector<RGB> color_map;

 private:
  FractalColoring(int exponent) : m_exponent(exponent) {}

  FractalColoring(int exponent, double bailout) : FractalColoring(exponent) {
    m_bailout = bailout;
  }
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_COLORING_HPP
