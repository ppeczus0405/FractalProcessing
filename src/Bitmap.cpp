#include <fstream>
#include "Bitmap.hpp"
#include "BitmapFileHeader.hpp"
#include "BitmapInfoHeader.hpp"
using namespace std;

Bitmap::Bitmap(int width, int height) : 
        m_width(width), m_height(height), 
        m_pixels(new uint8_t[width * height * 3]{}) 
{ }

void Bitmap::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue){
    if(x < 1 || x > m_width || y < 1 || y > m_height){
        throw new out_of_range("Can't set pixel. Pixel coordinates out of range");
    }
    x--; y--;
    int ind = y * m_width * 3 + x * 3;
    
    // BMP is Little Endian file format
    m_pixels[ind] = blue; 
    m_pixels[ind + 1] = green;
    m_pixels[ind + 2] = red;
}

bool Bitmap::write(string filename){
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    int pxArraySize = m_width * m_height * 3;
    
    fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pxArraySize;
    fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    infoHeader.width = m_width;
    infoHeader.height = m_height;

    fstream destination(filename, ios::out | ios::binary);

    if(!destination){ // Can't open file
        return false;
    }

    destination.write(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    destination.write(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));
    destination.write(reinterpret_cast<char *>(m_pixels.get()), pxArraySize);

    destination.close();
    if(!destination){ // Can't close the file
        return false;
    }

    return true;
}

Bitmap::~Bitmap(){

}