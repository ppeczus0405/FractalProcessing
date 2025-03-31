#include "ImageCanvas.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>
#include <cstring> // for memcpy

ImageCanvas::ImageCanvas(QWidget* parent)
    : QWidget(parent),
      image(1080, 720, QImage::Format_RGB888)
{
    setFixedSize(1080, 720);

    // We’ll create the rubber band once and keep it hidden until a drag begins
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->hide();
}

void ImageCanvas::updateImage(const uint8_t* imageData) {
    // Copy raw RGB bytes into our QImage
    for (int y = 0; y < image.height(); ++y) {
        std::memcpy(image.scanLine(y),
                    imageData + y * image.width() * 3,
                    image.width() * 3);
    }
    update(); // Repaint
}

void ImageCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
    // rubberBand is drawn automatically by Qt on top
}

void ImageCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        selectionStart = event->pos();
        // Reset rubberBand geometry to a zero‐sized rect at the click
        rubberBand->setGeometry(QRect(selectionStart, QSize()));
        rubberBand->show();
    }
}

void ImageCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (selecting) {
        // Update rubberBand geometry to follow the mouse
        QRect rect(selectionStart, event->pos());
        rubberBand->setGeometry(rect.normalized());
    }
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        selecting = false;
        rubberBand->hide();

        QRect selectedRect = rubberBand->geometry();
        if (!selectedRect.isNull()) {
            // Notify whoever is listening that we have a valid selection
            emit zoomAreaSelected(selectedRect);
        }
    }
}

