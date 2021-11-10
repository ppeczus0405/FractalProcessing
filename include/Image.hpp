#ifndef PEKI_IMAGE_HPP
#define PEKI_IMAGE_HPP

#include <string>
#include <memory>
#include <cstdint>
#include "RGB.hpp"
#include "ImageSaver.hpp"
using namespace std;

constexpr int RGB_SIZE = 3; // number of colors in RGB representation 

namespace PekiProcessing
{
    class Image {
        public:
            Image(int width, int height);
            void setPixel(int x, int y, const RGB &pixel);
            virtual bool write(const string &filename, const SaveFormat &format = SaveFormat::JPEG);
        protected:
            int m_width = 0;
            int m_height = 0;
            unique_ptr<uint8_t[]> m_pixels = nullptr;
            unique_ptr<ImageSaver> imgsaver = nullptr;
    };
}

#endif // PEKI_IMAGE_HPP