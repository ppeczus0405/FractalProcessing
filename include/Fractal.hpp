#ifndef PEKI_FRACTAL_HPP
#define PEKI_FRACTAL_HPP

#include "FractalAlgorithm.hpp"
#include "FractalColoring.hpp"
#include "Gradient.hpp"
#include "Image.hpp"
#include "Scale.hpp"

namespace PekiProc {

class FractalBuilder;
using FAT = FractalAlgorithmType;
using Dim = std::tuple<long double, long double, long double, long double>;

class Fractal : public Image {
  friend class FractalBuilder;

 public:
  bool resize(int width, int height);
  bool setRectangle(std::pair<int, int> v1, std::pair<int, int> v2);
  bool setScale(long double minR, long double maxR, long double minI,
                long double maxI, bool baseChanged = false);
  bool isPreviousScale();
  bool write(const std::string& filename,
             const SaveFormat& format = SaveFormat::JPEG) override;

  RGB getPixel(int x, int y) override;
  const uint8_t* getData() override;


  void setPreviousScale();
  void setDefaultScale();
  void setIterations(int iters);
  void setAlgorithm(std::unique_ptr<FractalAlgorithm> alg);
  void setGradientMapSize(int mapSize);

  template <typename T>
  void setGradient(T&& gradient) {
    static_assert(std::is_constructible<Gradient, T>::value,
                  "Cannot construct Gradient object from this type");
    isGenerated = false;
    fcol->setGradient(make_unique<Gradient>(forward<T>(gradient)));
  }

 private:
  Fractal(int width, int height);

  bool isGenerated = false;
  std::vector<Dim> scaleStack;
  std::unique_ptr<FractalAlgorithm> falg = nullptr;
  std::unique_ptr<FractalColoring> fcol = nullptr;
  std::unique_ptr<Scale> scale = nullptr;

  void generate();

  static constexpr Dim DefaultMandelbrotDim = Dim(-2.2L, 1.0L, -1.2L, 1.2L);
  static constexpr Dim DefaultMultibrotDim = Dim(-2.2L, 2.2L, -1.65L, 1.65L);
  static constexpr Dim DefaultJuliaDim = Dim(-2.0L, 2.0L, -1.5L, 1.5L);
  static constexpr Dim DefaultNewtonDim = Dim(-2.8L, 2.8L, -2.1L, 2.1L);
  static constexpr Dim DefaultPolyJuliaDim = DefaultMultibrotDim;
  static constexpr Dim DefaultNovaDim = DefaultJuliaDim;
  static Dim getDefaultDimension(const FAT alg) noexcept;
};

class FractalBuilder {
 public:
  FractalBuilder(int width, int height);

  FractalBuilder& setScale(long double minR, long double maxR, long double minI,
                           long double maxI);
  FractalBuilder& setAlgorithm(std::unique_ptr<FractalAlgorithm> alg);
  FractalBuilder& setMaxIterations(int mxIter);
  FractalBuilder& setGradientMapSize(int size);

  std::unique_ptr<Fractal> build();

  template <typename T>
  FractalBuilder& setGradient(T&& g) {
    static_assert(std::is_constructible<Gradient, T>::value,
                  "Cannot construct Gradient object from this type");
    gradient = make_unique<Gradient>(forward<T>(g));
    return *this;
  }

 private:
  int maxIterations = -1;
  int mapSize = -1;
  std::unique_ptr<Gradient> gradient = nullptr;
  std::unique_ptr<Fractal> fractal = nullptr;
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_HPP
