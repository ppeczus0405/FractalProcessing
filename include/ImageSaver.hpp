#ifndef IMAGE_SAVER_HPP
#define IMAGE_SAVER_HPP

#include <Magick++.h>
#include <map>
#include <string>
#include <memory>
using namespace std;


constexpr int DEFAULT_SAVE_WIDTH = 600;
constexpr int DEFAULT_SAVE_HEIGHT = 600;

/**
 * @brief Possible image file formats 
 * 
 */
enum class SaveFormat{
    JPEG,
    PNG,
    BMP,
    GIF,
    TIFF,
    NONE
};

/**
 * @brief Saves images
 * 
 */
class ImageSaver
{
    public:
        /**
         * @brief Construct a new Image Saver object
         * 
         * @param data - pixels of the image
         * @param width - width of the image
         * @param height - height of the image
         * @param format - save format(e.g. PNG, JPG, GIFF)
         */
        ImageSaver(const char * data, int width = DEFAULT_SAVE_HEIGHT, int height = DEFAULT_SAVE_HEIGHT, 
                   SaveFormat format = SaveFormat::JPEG);
        
        /**
         * @brief Saving image to file
         * 
         * @param filename - path where file is going to save
         * @return true when successfully saved the file
         * @return false otherwise 
         */
        bool save(const string &filename);

        /**
         * @brief Change saving format
         * 
         * @param format - new format
         * @return true when successfully changed saving format
         * @return false otherwise 
         */
        bool changeFormat(const SaveFormat &format);
    private:
        SaveFormat m_format = SaveFormat::NONE;
        unique_ptr<Magick::Image> saveImage = nullptr;
        map <SaveFormat, pair <string, string> > extMap;
};

#endif // IMAGE_SAVER_HPP