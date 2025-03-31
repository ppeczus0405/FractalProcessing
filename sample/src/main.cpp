#include <QApplication>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>

#include "FractalParameterPanel.hpp"
#include "ImageCanvas.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  window.setFixedSize(1380, 720);
  window.setWindowTitle("Fractal Generator");

  QHBoxLayout* mainLayout = new QHBoxLayout(&window);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // === Side Panel (300px) ===
  QWidget* sidePanel = new QWidget;
  sidePanel->setFixedWidth(300);
  sidePanel->setStyleSheet(R"(
        background-color: #1e1e1e;
        color: #f0f0f0;
        font-size: 13px;
        QLineEdit, QSpinBox, QDoubleSpinBox, QListWidget {
            background-color: #2e2e2e;
            color: #f0f0f0;
            border: 1px solid #555;
        }
        QLabel {
            color: #cccccc;
        }
        QCheckBox, QComboBox, QPushButton {
            background-color: #333;
            color: #f0f0f0;
            border: 1px solid #444;
        }
        QPushButton:hover {
            background-color: #444;
        }
    )");

  QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
  FractalParameterPanel* parameterPanel = new FractalParameterPanel;
  sideLayout->addWidget(parameterPanel);

  // Zoom Out button (disabled until there's a previous scale)
  QPushButton* zoomOutButton = new QPushButton("Zoom Out");
  zoomOutButton->setEnabled(false);
  sideLayout->addWidget(zoomOutButton);

  // Stretch so things stay top‐aligned
  sideLayout->addStretch(1);


  // === Image Canvas ===
  ImageCanvas* canvas = new ImageCanvas;

  // === Layout Composition ===
  mainLayout->addWidget(sidePanel);
  mainLayout->addWidget(canvas);

  // === Signal connection (placeholder) ===
  FractalGenerator fractalGenerator;
  QObject::connect(parameterPanel, &FractalParameterPanel::generateFractal,
                   [&]() {
                     if (!parameterPanel->validateInputs()) {
                       QMessageBox::warning(nullptr, "Invalid Input",
                                            "There are missing fields to fill");
                       return;
                     }
                     fractalGenerator.update(parameterPanel->collectParameters());
                     canvas->updateImage(fractalGenerator.imageData());

                     // If we just generated from scratch, we are at the base scale:
                     zoomOutButton->setEnabled(fractalGenerator.isPreviousScale());
                   });

  // Respond to user’s selection for zooming
  QObject::connect(canvas, &ImageCanvas::zoomAreaSelected,
                   [&](const QRect &r) {
                       // The fractal library expects 1-based coordinates.
                       // (x1,y1) is typically bottom-left, (x2,y2) top-right in the library’s logic.
                       // But Qt has (0,0) at top-left, so we must adapt carefully.

                       // Convert from widget coords to 1-based:
                       int x1 = r.left()   + 1;
                       int y1 = r.top()    + 1;
                       int x2 = r.right()  + 1;
                       int y2 = r.bottom() + 1;

                       // Use fractalGenerator’s internal fractal->setRectangle(...) function
                       bool success = fractalGenerator.zoom({x1, y1}, {x2, y2});
                       if (!success) {
                           QMessageBox::warning(&window, "Zoom Error",
                                                "The selected rectangle was invalid.");
                           return;
                       }
                       // Library regenerates automatically or on next getData.
                       // Just refresh the canvas
                       canvas->updateImage(fractalGenerator.imageData());

                       // Now there's definitely a previous scale, so enable Zoom Out
                       zoomOutButton->setEnabled(fractalGenerator.isPreviousScale());
                   });

  // Zoom Out button: revert to the previous scale
  QObject::connect(zoomOutButton, &QPushButton::clicked, [&]() {
      if (fractalGenerator.isPreviousScale()) {
          fractalGenerator.setPreviousScale();
          canvas->updateImage(fractalGenerator.imageData());

          // If no more stacked zoom, disable button
          zoomOutButton->setEnabled(fractalGenerator.isPreviousScale());
      }
  });
  window.show();
  return app.exec();
}
