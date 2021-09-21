#include <iostream>
#include "ImageSaver.hpp"

ImageSaver::ImageSaver(const char * data, int width, int height, SaveFormat format) :
            saveImage(make_unique <Magick::Image> (width, height, "RGB", MagickCore::StorageType::CharPixel, data))
{
    changeFormat(format);
    extMap[SaveFormat::JPEG] = {".jpg", "JPEG"};
    extMap[SaveFormat::PNG]  = {".png", "PNG" };
    extMap[SaveFormat::BMP]  = {".bmp", "BMP" };
    extMap[SaveFormat::GIF]  = {".gif", "GIF" };
    extMap[SaveFormat::TIFF] = {".tif", "TIFF"};
}

bool ImageSaver::save(const string& filename){
    if(m_format == SaveFormat::NONE){
        cerr << "Specify format before using save function" << endl;
        return false;
    }
    try{
        saveImage->write(filename + extMap[m_format].first);
    }
    catch(exception &exp){
        cerr << "Can't save image properly. Reason: " << exp.what() << endl;
        return false;
    }
    return true;
}

bool ImageSaver::changeFormat(const SaveFormat &format){
    if(format == m_format || format == SaveFormat::NONE){
        m_format = format; // NONE case
        return true;
    }
    try{
        saveImage->magick(extMap[format].second);
        m_format = format;
    }
    catch(exception &exp){
        cerr << "Can't set image format properly. Reason: " << exp.what() << endl;
        return false;
    }
    return true;
}