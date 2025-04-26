#ifndef PEKI_FRACTAL_COLORING_HPP
#define PEKI_FRACTAL_COLORING_HPP

#include "Complex.hpp"
#include "Gradient.hpp"
#include "RGB.hpp"
#include "UtilsGPU.hpp"

namespace PekiProc {

enum class FractalColoringType {
  SMOOTH_CONVERGENCE,
  SMOOTH_DIVERGENCE,
};

struct FractalColoringConfiguration {
  FractalColoringType coloringType;
  int maxIterations;
  int exponent;
  double bailout;
  int mapSize;
  RGB* colorMap = nullptr;  // device pointer!

  CUDA_HD void dumpConfig() const {
#if CUDA_COMPATIBLE
    printf("[DEVICE] Coloring cfg: type=%d, iter=%d, exp=%d, bailout=%f, map=%d\n",
           static_cast<int>(coloringType), maxIterations, exponent, bailout,
           mapSize);
#else
    std::cout << "Coloring Type: " << static_cast<int>(coloringType)
              << "\nMax Iterations: " << maxIterations
              << "\nExponent: " << exponent << "\nBailout: " << bailout
              << "\nMap size: " << mapSize << std::endl;
#endif
  }
};

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
    color_config.maxIterations = max_iterations;
  }

  void setGradient(std::unique_ptr<Gradient> gradient) {
    if (gradient) {
      m_gradient = std::move(gradient);
      color_map = m_gradient->generateGradientMap(map_size);
      color_config.colorMap = color_map.data();
    }
  }

  std::unique_ptr<Gradient> getGradient() noexcept {
    return std::move(m_gradient);
  }

  int getColorMapSize() const noexcept { return map_size; }

  const RGB* getColorMap() const noexcept { return color_map.data(); }

  bool setColorMapSize(int mapSize) {
    bool status = true;
    if (mapSize >= MIN_MAP_SIZE && mapSize <= MAX_MAP_SIZE) {
      if (map_size != mapSize) {
        map_size = mapSize;
        color_map = m_gradient->generateGradientMap(map_size);
      } else {
        status = false;
      }
    } else {
      std::cerr << "Map size has to meet the condition: ";
      std::cerr << MIN_MAP_SIZE << " <= size <= " << MAX_MAP_SIZE << std::endl;
      if (map_size == -1) {
        std::cerr << "Setted map size to " << DEFAULT_COLOR_MAP_SIZE
                  << std::endl;
        map_size = DEFAULT_COLOR_MAP_SIZE;
        color_map = m_gradient->generateGradientMap(map_size);
      } else {
        std::cerr << "Size not changed" << std::endl;
        status = false;
      }
    }
    color_config.mapSize = getColorMapSize();
    color_config.colorMap = color_map.data();
    return status;
  }

  void setMaxIterations(int iters) {
    color_config.maxIterations = max_iterations = iters;
  }

  const FractalColoringConfiguration& getFractalColoringConfig() const {
    return color_config;
  }

  static constexpr int DEFAULT_COLOR_MAP_SIZE = 512;
  static constexpr int MAX_MAP_SIZE = 16384;
  static constexpr int MIN_MAP_SIZE = 1;

  virtual RGB getPixel(
      const PairGPU<int, TripleGPU<Complex, Complex, Complex>>& iterOrbit) = 0;

  virtual ~FractalColoring() = default;

 protected:
  // Util function used to determine color map index.
  template <typename T>
  CUDA_HD static constexpr T clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (v > hi ? hi : v);
  }

  double m_bailout;

  int max_iterations;
  int m_exponent;
  int map_size = -1;

  std::unique_ptr<Gradient> m_gradient = nullptr;
  std::vector<RGB> color_map;

  FractalColoringConfiguration color_config;

 private:
  FractalColoring(int exponent) : m_exponent(exponent) {
    color_config.exponent = m_exponent;
  }

  FractalColoring(int exponent, double bailout) : FractalColoring(exponent) {
    color_config.bailout = m_bailout = bailout;
  }
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_COLORING_HPP
