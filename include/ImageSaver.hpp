#ifndef PEKI_IMAGE_SAVER_HPP
#define PEKI_IMAGE_SAVER_HPP

#include <map>
#include <memory>
#include <string>
#include "Magick++/Image.h"

namespace PekiProc {

enum class SaveFormat { JPEG, PNG, BMP, GIF, TIFF, NONE };

class ImageSaver {
 public:
  ImageSaver(const char* data, int width = DEFAULT_SAVE_HEIGHT,
             int height = DEFAULT_SAVE_HEIGHT,
             SaveFormat format = SaveFormat::JPEG);

  bool save(const std::string& filename);
  bool changeFormat(const SaveFormat& format);

  static constexpr int DEFAULT_SAVE_WIDTH = 600;
  static constexpr int DEFAULT_SAVE_HEIGHT = 600;

 private:
  SaveFormat m_format = SaveFormat::NONE;
  std::unique_ptr<Magick::Image> saveImage = nullptr;
  std::map<SaveFormat, std::pair<std::string, std::string>> extMap;
};

}  // namespace PekiProc

#endif  // PEKI_IMAGE_SAVER_HPP
