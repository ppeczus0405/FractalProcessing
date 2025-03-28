#include <cassert>
#include <cstring>

#include "Image.hpp"

namespace PekiProc {

Image::Image(int width, int height) : m_width(width), m_height(height) {
  assert(width > 0 && height > 0);
  m_pixels = std::make_unique<uint8_t[]>(m_width * m_height * 3);
  memset(m_pixels.get(), 0, m_width * m_height * 3);
}

void Image::setPixel(int x, int y, const RGB& pixel) {
  assert(x >= 1 && x <= m_width && y >= 1 && y <= m_height);
  x--;
  y--;
  int ind = y * m_width * 3 + x * 3;
  m_pixels[ind] = pixel.getR();
  m_pixels[ind + 1] = pixel.getG();
  m_pixels[ind + 2] = pixel.getB();
}

RGB Image::getPixel(int x, int y) {
  assert(x >= 1 && x <= m_width && y >= 1 && y <= m_height);
  x--;
  y--;
  int ind = y * m_width * 3 + x * 3;
  return RGB(m_pixels[ind], m_pixels[ind + 1], m_pixels[ind + 2]);
}

const uint8_t* Image::getData() {
  return m_pixels.get();
}

bool Image::write(const std::string& filename, const SaveFormat& format) {
  imgsaver = std::make_unique<ImageSaver>(
      reinterpret_cast<const char*>(m_pixels.get()), m_width, m_height, format);
  if (!imgsaver->changeFormat(format)) {
    return false;
  }
  return imgsaver->save(filename);
}

}  // namespace PekiProc
