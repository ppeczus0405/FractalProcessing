#ifndef IMAGECANVAS_HPP
#define IMAGECANVAS_HPP

#include <QImage>
#include <QWidget>

class ImageCanvas : public QWidget {
  Q_OBJECT

 public:
  explicit ImageCanvas(QWidget* parent = nullptr);
  void updateImage(const uint8_t* imageData);

 protected:
  void paintEvent(QPaintEvent*) override;

 private:
  QImage image;
};

#endif  // IMAGECANVAS_HPP
