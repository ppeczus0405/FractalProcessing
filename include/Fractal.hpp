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
using Dim = std::tuple<double, double, double, double>;

class Fractal : public Image {
  friend class FractalBuilder;

 public:
  bool resize(int width, int height);
  bool setRectangle(std::pair<int, int> v1, std::pair<int, int> v2);
  bool setScale(double minR, double maxR, double minI,
                double maxI, bool baseChanged = false);
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
    fcol->setGradient(std::make_unique<Gradient>(std::forward<T>(gradient)));
  }

  static Dim getDefaultDimension(const FAT alg) noexcept;

 private:
  Fractal(int width, int height);

  bool isGenerated = false;
  std::vector<Dim> scaleStack;
  std::unique_ptr<FractalAlgorithm> falg = nullptr;
  std::unique_ptr<FractalColoring> fcol = nullptr;
  std::unique_ptr<Scale> scale = nullptr;

  void generate();

  static constexpr Dim DefaultMandelbrotDim = Dim(-2.2, 1.0, -1.2, 1.2);
  static constexpr Dim DefaultMultibrotDim = Dim(-2.2, 2.2, -1.65, 1.65);
  static constexpr Dim DefaultJuliaDim = Dim(-2.0, 2.0, -1.5, 1.5);
  static constexpr Dim DefaultNewtonDim = Dim(-2.8, 2.8, -2.1, 2.1);
  static constexpr Dim DefaultPolyJuliaDim = DefaultMultibrotDim;
  static constexpr Dim DefaultNovaDim = DefaultJuliaDim;
};

class FractalBuilder {
 public:
  FractalBuilder(int width, int height);

  FractalBuilder& setScale(double minR, double maxR, double minI,
                           double maxI);
  FractalBuilder& setScale(const Dim& scale);
  FractalBuilder& setAlgorithm(std::unique_ptr<FractalAlgorithm> alg);
  FractalBuilder& setMaxIterations(int mxIter);
  FractalBuilder& setGradientMapSize(int size);

  std::unique_ptr<Fractal> build();

  template <typename T>
  FractalBuilder& setGradient(T&& g) {
    static_assert(std::is_constructible<Gradient, T>::value,
                  "Cannot construct Gradient object from this type");
    gradient = std::make_unique<Gradient>(std::forward<T>(g));
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
