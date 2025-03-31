#include <cstring>

#include "Fractal.hpp"
#include "FractalAlgorithmCreator.hpp"
#include "FractalColoringCreator.hpp"
#include "Image.hpp"

namespace PekiProc {

Dim Fractal::getDefaultDimension(const FAT algType) noexcept {
  switch (algType) {
    case FAT::MANDELBROT:
      return DefaultMandelbrotDim;
    case FAT::MULTIBROT:
      return DefaultMultibrotDim;
    case FAT::JULIA:
      return DefaultJuliaDim;
    case FAT::NEWTON:
      return DefaultNewtonDim;
    case FAT::POLYJULIA:
      return DefaultPolyJuliaDim;
    default:
      return DefaultNovaDim;
  }
}

// Fractal
Fractal::Fractal(int width, int height) : Image(width, height), scaleStack(1) {}

bool Fractal::resize(int width, int height) {
  if (width == m_width && height == m_height)
    return true;
  if (width <= 0 or height <= 0) {
    std::cerr
        << "Can't resize. Dimensions have to be positive integers. Size not "
           "changed"
        << std::endl;
    return false;
  }
  // Trying to change scale, if we can't then restore previous state
  int prev_width = m_width, prev_height = m_height;
  m_width = width;
  m_height = height;
  if (!setScale(scale->getMinReal(), scale->getMaxReal(), scale->getMinImag(),
                scale->getMaxImag(), true)) {
    m_width = prev_width;
    m_height = prev_height;
    return false;
  }

  // Initialize new pixel table
  isGenerated = false;
  m_pixels = std::make_unique<uint8_t[]>(m_width * m_height * 3);
  memset(m_pixels.get(), 0, m_width * m_height * 3);
  return true;
}

// (0, 0) coordinate is top left corner
bool Fractal::setRectangle(std::pair<int, int> v1, std::pair<int, int> v2) {
  // Degenerate rectangle
  if (v1.first == v2.first or v1.second == v2.second) {
    std::cerr << "Given rectangle has surface area = 0" << std::endl;
    return false;
  }

  if (v1.first > v2.first)
    swap(v1, v2);
  auto [xa, ya] = v1;
  auto [xb, yb] = v2;
  bool notValid = xa < 1 or xa > m_width or xb < 1 or xb > m_width;
  notValid |= ya < 1 or ya > m_height or yb < 1 or yb > m_height;

  // Out of bounds
  if (notValid) {
    std::cerr
        << "Given rectangle is not valid. There is at least one coordinate "
           "out of bounds image"
        << std::endl;
    return false;
  }

  // Normalize to form when (xa, ya) - left bottom corner, (xb, yb) - right top
  // corner
  if (ya < yb)
    std::swap(ya, yb);

  // bottomScaled.first = minReal, bottomScaled.second = maxImag
  auto bottomScaled = scale->getScaled(xa, ya);

  // topScaled.first = maxReal, topScaled.second = minImag
  auto topScaled = scale->getScaled(xb, yb);

  long double mr = bottomScaled.first, Mr = topScaled.first;
  long double mi = topScaled.second, Mi = bottomScaled.second;

  if (setScale(mr, Mr, mi, Mi)) {
    scaleStack.emplace_back(mr, Mr, mi, Mi);
    return true;
  }
  return false;
}

bool Fractal::isPreviousScale() {
  return (int)scaleStack.size() > 1;
}

bool Fractal::setScale(long double minR, long double maxR, long double minI,
                       long double maxI, bool baseChanged) {
  bool sameScales = CompareDoubles::isEqual(minR, scale->getMinReal()) and
                    CompareDoubles::isEqual(maxR, scale->getMaxReal()) and
                    CompareDoubles::isEqual(minI, scale->getMinImag()) and
                    CompareDoubles::isEqual(maxI, scale->getMaxImag());
  if (!sameScales || baseChanged) {
    try {
      std::unique_ptr<Scale> ps =
          std::make_unique<Scale>(m_width, m_height, minR, maxR, minI, maxI);
      scale = std::move(ps);
      isGenerated = false;
    } catch (const std::invalid_argument& invArgument) {
      std::cerr << invArgument.what() << std::endl;
      return false;
    }
  }
  return true;
}

bool Fractal::write(const std::string& filename, const SaveFormat& format) {
  if (!isGenerated)
    generate();
  return Image::write(filename, format);
}

RGB Fractal::getPixel(int x, int y) {
  if (!isGenerated)
    generate();
  return Image::getPixel(x, y);
}

const uint8_t* Fractal::getData() {
  if (!isGenerated)
    generate();
  return Image::getData();
}

void Fractal::setPreviousScale() {
  if (!isPreviousScale())
    return;
  scaleStack.pop_back();
  auto [mr, Mr, mi, Mi] = scaleStack.back();
  setScale(mr, Mr, mi, Mi);
}

void Fractal::setDefaultScale() {
  scaleStack.clear();
  Dim defaultScale = getDefaultDimension(falg->getAlgorithmType());
  scaleStack.push_back(defaultScale);
  auto [mr, Mr, mi, Mi] = defaultScale;
  setScale(mr, Mr, mi, Mi);
}

void Fractal::setIterations(int iters) {
  if (falg->setMaxIterationsNumber(iters)) {
    fcol->setMaxIterations(iters);
    isGenerated = false;
  }
}

void Fractal::setAlgorithm(std::unique_ptr<FractalAlgorithm> alg) {
  if (alg) {
    auto checkConvergence = [](auto algtype) {
      return algtype == FAT::NEWTON or algtype == FAT::NOVA;
    };

    auto getColoring = [this](auto& creator) {
      return creator(falg->getMaxIterationsNumber(), falg->getExponent(),
                     fcol->getColorMapSize(), fcol->getGradient());
    };

    falg = std::move(alg);
    bool isCurrentConvergence = checkConvergence(falg->getAlgorithmType());

    if (isCurrentConvergence)
      fcol = getColoring(FractalColoringCreator::createSmoothConvergence);
    else
      fcol = getColoring(FractalColoringCreator::createSmoothDivergence);

    setDefaultScale();
    isGenerated = false;
  } else
    std::cerr << "Given algorithm is nullpointer. Not changed" << std::endl;
}

void Fractal::setGradientMapSize(int mapSize) {
  if (fcol->setColorMapSize(mapSize))
    isGenerated = false;
}

void Fractal::generate() {
  if (isGenerated)
    return;
  for (int i = 1; i <= m_width; i++) {
    for (int j = 1; j <= m_height; j++) {
      auto result = scale->getScaled(i, j);
      Complex c(result.first, result.second);
      setPixel(i, j, fcol->getPixel(falg->getIterationsAndOrbit(c)));
    }
  }
  isGenerated = true;
}

// FractalBuilder
FractalBuilder::FractalBuilder(int width, int height)
    : fractal(new Fractal(width, height)) {}

FractalBuilder& FractalBuilder::setScale(long double minR, long double maxR,
                                         long double minI, long double maxI) {
  return setScale(Dim{minR, maxR, minI, maxI});
}

FractalBuilder& FractalBuilder::setScale(const Dim& scale) {
  try {
    auto [minR, maxR, minI, maxI] = scale;
    std::unique_ptr<Scale> ps = std::make_unique<Scale>(
        fractal->m_width, fractal->m_height, minR, maxR, minI, maxI);
    fractal->scale = std::move(ps);
    fractal->scaleStack.front() = Dim(minR, maxR, minI, maxI);
  } catch (const std::invalid_argument& invArgument) {
    std::cerr << invArgument.what() << std::endl;
  }
  return *this;
}

FractalBuilder& FractalBuilder::setAlgorithm(
    std::unique_ptr<FractalAlgorithm> alg) {
  fractal->falg = std::move(alg);
  return *this;
}

FractalBuilder& FractalBuilder::setMaxIterations(int mxIter) {
  maxIterations = mxIter;
  return *this;
}

FractalBuilder& FractalBuilder::setGradientMapSize(int size) {
  mapSize = size;
  return *this;
}

std::unique_ptr<Fractal> FractalBuilder::build() {
  if (fractal->falg == nullptr) {
    fractal->falg = FractalAlgorithmCreator::createMandelbrot();
  }
  fractal->falg->setMaxIterationsNumber(maxIterations);
  FAT algType = fractal->falg->getAlgorithmType();
  bool isConvergence = (algType == FAT::NEWTON) or (algType == FAT::NOVA);

  auto getColoring = [this](auto& creator) {
    return creator(fractal->falg->getMaxIterationsNumber(),
                   fractal->falg->getExponent(), mapSize, std::move(gradient));
  };

  if (isConvergence)
    fractal->fcol =
        getColoring(FractalColoringCreator::createSmoothConvergence);
  else
    fractal->fcol = getColoring(FractalColoringCreator::createSmoothDivergence);

  if (fractal->scale == nullptr) {
    auto [mr, Mr, mi, Mi] = Fractal::getDefaultDimension(algType);
    fractal->scale = std::make_unique<Scale>(fractal->m_width,
                                             fractal->m_height, mr, Mr, mi, Mi);
    fractal->scaleStack.front() = Dim(mr, Mr, mi, Mi);
  }
  return std::move(fractal);
}

}  // namespace PekiProc
