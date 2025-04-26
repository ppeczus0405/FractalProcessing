#ifndef IMAGECANVAS_HPP
#define IMAGECANVAS_HPP

#include <QImage>
#include <QPoint>
#include <QRect>
#include <QWidget>

class QRubberBand;

class ImageCanvas : public QWidget {
  Q_OBJECT
 public:
  explicit ImageCanvas(QWidget* parent = nullptr);
  void updateImage(const uint8_t* imageData);

 signals:
  // Emitted when the user finishes selecting a zoom region
  void zoomAreaSelected(const QRect& selectionRect);

 protected:
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  QImage image;
  bool selecting = false;
  QPoint selectionStart;
  QRubberBand* rubberBand = nullptr;
};

#endif  // IMAGECANVAS_HPP
