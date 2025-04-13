#ifndef PEKI_IMAGE_HPP
#define PEKI_IMAGE_HPP

#include <cstdint>
#include <memory>
#include <string>

#include "ImageSaver.hpp"
#include "RGB.hpp"

namespace PekiProc {

class Image {
 public:
  Image(int width, int height);

  void setPixel(int x, int y, const RGB& pixel);

  virtual RGB getPixel(int x, int y);
  virtual const uint8_t* getData();

  virtual bool write(const std::string& filename,
                     const SaveFormat& format = SaveFormat::JPEG);

  virtual ~Image() = default;

 protected:
  int m_width = 0;
  int m_height = 0;

  std::unique_ptr<uint8_t[]> m_pixels = nullptr;
  std::unique_ptr<ImageSaver> imgsaver = nullptr;
};

}  // namespace PekiProc

#endif  // PEKI_IMAGE_HPP
