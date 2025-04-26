#ifndef FRACTALACTIONPANEL_HPP
#define FRACTALACTIONPANEL_HPP

#include <QWidget>

class QPushButton;
class QLabel;

class FractalActionPanel : public QWidget {
  Q_OBJECT
 public:
  explicit FractalActionPanel(QWidget* parent = nullptr);

 signals:
  // Emitted when user clicks "Generate"
  void generateFractal();

  // Emitted when user clicks "Zoom Out"
  void zoomOutRequest();

 public slots:
  // Update the info label text
  void setInfoText(const QString& text);

  // Update the text color of Zoom Out label
  // e.g. if no zoom out is possible, we color it gray
  void setZoomOutAvailable(bool available);

 private:
  QPushButton* m_generateButton;
  QLabel* m_infoLabel;
  QPushButton* m_zoomOutButton;
};

#endif  // FRACTALACTIONPANEL_HPP
