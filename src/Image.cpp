#include "Image.hpp"
#include <assert.h>
#include <cstring>
using namespace PekiProcessing;

Image::Image(int width, int height) : 
       m_width(width), m_height(height)
{
    assert(width > 0 && height > 0);
    m_pixels = make_unique<uint8_t[]>(m_width * m_height * 3);
    memset(m_pixels.get(), 0, m_width * m_height * 3);
}

void Image::setPixel(int x, int y, const RGB &pixel)
{
    assert(x >= 1 && x <= m_width && y >= 1 && y <= m_height);
    x--; y--;
    int ind = y * m_width * 3 + x * 3;
    m_pixels[ind] = pixel.getR();
    m_pixels[ind + 1] = pixel.getG();
    m_pixels[ind + 2] = pixel.getB();
}

bool Image::write(const string &filename, const SaveFormat &format)
{
    if(!imgsaver){
        imgsaver = make_unique<ImageSaver>(reinterpret_cast <const char *>(m_pixels.get()), m_width, m_height, format);
    }
    if(!imgsaver->changeFormat(format)){
        return false;
    }
    return imgsaver->save(filename);
}