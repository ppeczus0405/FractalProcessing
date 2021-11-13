#ifndef PEKI_IMAGE_HPP
#define PEKI_IMAGE_HPP

#include <string>
#include <memory>
#include <cstdint>
#include "RGB.hpp"
#include "ImageSaver.hpp"
using namespace std;

/**
 * @brief number of colors in RGB representation 
 * 
 */
constexpr int RGB_SIZE = 3;

namespace PekiProcessing
{
    /**
     * @brief Stores and set pixels
     */
    class Image {
        public:
            /**
             * @brief Construct a new image object.
             * 
             * @param width - width of the image
             * @param height - height of the image
             */
            Image(int width, int height);

            /**
             * @brief Set the pixel at position
             * 
             * 
             * @param x - coordinate on the OX axis, the following condition has to be met: 1 <= x <= width
             * @param y - coordinate on the OY axis, the following condition has to be met: 1 <= y <= height
             * @param pixel - color of the pixel in RGB representation
             * @note Point (1, 1) is the top left corner. Point (width, height) is the right bottom corner.
             */
            void setPixel(int x, int y, const RGB &pixel);


            /**
             * @brief Save image to file
             * 
             * @param filename - path where file is going to save
             * @param format - save format(e.g. PNG, JPG, GIFF)
             * @return true when successfully saved the file
             * @return false otherwise
             */
            virtual bool write(const string &filename, const SaveFormat &format = SaveFormat::JPEG);
        protected:
            int m_width = 0;
            int m_height = 0;
            unique_ptr<uint8_t[]> m_pixels = nullptr;
            unique_ptr<ImageSaver> imgsaver = nullptr;
    };
}

#endif // PEKI_IMAGE_HPP