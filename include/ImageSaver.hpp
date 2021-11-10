#ifndef IMAGE_SAVER_HPP
#define IMAGE_SAVER_HPP

#include <Magick++.h>
#include <map>
#include <string>
#include <memory>
using namespace std;

constexpr int DEFAULT_SAVE_WIDTH = 600;
constexpr int DEFAULT_SAVE_HEIGHT = 600;

enum class SaveFormat{
    JPEG,
    PNG,
    BMP,
    GIF,
    TIFF,
    NONE
};

class ImageSaver
{
    public:
        ImageSaver(const char * data, int width = DEFAULT_SAVE_HEIGHT, int height = DEFAULT_SAVE_HEIGHT, 
                   SaveFormat format = SaveFormat::JPEG);
        bool save(const string &filename);
        bool changeFormat(const SaveFormat &format);
    private:
        SaveFormat m_format = SaveFormat::NONE;
        unique_ptr<Magick::Image> saveImage = nullptr;
        map <SaveFormat, pair <string, string> > extMap;
};

#endif // IMAGE_SAVER_HPP