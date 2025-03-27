#include <QPainter>
#include <cstring>

#include "ImageCanvas.hpp"

ImageCanvas::ImageCanvas(QWidget* parent)
    : QWidget(parent), image(1080, 720, QImage::Format_RGB888) {
  setFixedSize(1080, 720);
}

void ImageCanvas::updateImage(const uint8_t* data) {
  for (int y = 0; y < image.height(); ++y) {
    memcpy(image.scanLine(y), data + y * image.width() * 3, image.width() * 3);
  }
  update();
}

void ImageCanvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}
