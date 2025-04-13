#ifndef PEKI_RGB_HPP
#define PEKI_RGB_HPP

#include <cstdint>
#include <iostream>

#include "CudaCompat.hpp"

namespace PekiProc {
class RGB;
}

// We need to override global operator so forward declare it before.
inline std::ostream& operator<<(std::ostream& stream,
                                const PekiProc::RGB& color);

namespace PekiProc {

class RGB {
 public:
  CUDA_HD RGB() : r(0), g(0), b(0) {}

  CUDA_HD RGB(uint8_t R, uint8_t G, uint8_t B) : r(R), g(G), b(B) {}

  const static RGB WHITE, GRAY, BLACK, RED, GREEN, BLUE;

  static constexpr int RGB_SIZE = 3;

  CUDA_HD uint8_t getR() const { return r; }

  CUDA_HD uint8_t getG() const { return g; }

  CUDA_HD uint8_t getB() const { return b; }

  CUDA_HD void setR(uint8_t c) { r = c; }

  CUDA_HD void setG(uint8_t c) { g = c; }

  CUDA_HD void setB(uint8_t c) { b = c; }

  friend std::ostream& ::operator<<(std::ostream& stream, const RGB& color);

 private:
  uint8_t r, g, b;
};

}  // namespace PekiProc

std::ostream& operator<<(std::ostream& stream, const PekiProc::RGB& color) {
  stream << "RGB(" << (int)color.r << ", " << (int)color.g << ", "
         << (int)color.b << ")";
  return stream;
}

#endif  // PEKI_RGB_HPP
