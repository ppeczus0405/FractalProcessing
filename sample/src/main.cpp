#include <QApplication>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>

#include "FractalParameterPanel.hpp"
#include "ImageCanvas.hpp"
#include "FractalActionPanel.hpp"
#include "FractalUI.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  window.setFixedSize(1380, 720);
  window.setWindowTitle("Fractal Generator");
  window.setObjectName("mainWindow"); // Give it a unique name
  window.setStyleSheet(R"(
    #mainWindow {
        background-color: #2e2e2e;  /* Your desired color */
    }
  )");

  QHBoxLayout* mainLayout = new QHBoxLayout(&window);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // Left side: a single widget that has param panel + action panel
  FractalUI* fractalUI = new FractalUI;
  fractalUI->setFixedWidth(300);
  mainLayout->addWidget(fractalUI);

  // Right side: the fractal display
  ImageCanvas* canvas = new ImageCanvas;
  mainLayout->addWidget(canvas);

  // Get pointers to the sub-panels
  auto* paramPanel = fractalUI->parameterPanel();
  auto* actionPanel = fractalUI->actionPanel();

  // Create your fractal generator
  FractalGenerator fractalGenerator;

  // =====================
  //  WIRING THINGS UP
  // =====================

  // 1) The "Generate" button in FractalActionPanel
  QObject::connect(actionPanel, &FractalActionPanel::generateFractal,
                   [&]() {
      // Validate user inputs from the param panel
      if (!paramPanel->validateInputs()) {
          QMessageBox::warning(nullptr, "Invalid Input", "There are missing fields to fill.");
          return;
      }
      // Show info that we're generating
      actionPanel->setInfoText("Generating fractal...");

      // Collect user parameters and update the fractal
      fractalGenerator.update(paramPanel->collectParameters());

      // Put the newly computed fractal on the canvas
      canvas->updateImage(fractalGenerator.imageData());

      // Done generating
      actionPanel->setInfoText("Done!");

      // Possibly update "Zoom Out" color if there's a previous scale
      bool canZoomOut = fractalGenerator.isPreviousScale();
      actionPanel->setZoomOutAvailable(canZoomOut);
  });

  // 2) Handling user’s drag-selection on the fractal
  QObject::connect(canvas, &ImageCanvas::zoomAreaSelected,
                   [&](const QRect &r) {
      actionPanel->setInfoText("Zooming...");

      // Convert from 0-based widget coords to 1-based library coords
      int x1 = r.left()   + 1;
      int y1 = r.top()    + 1;
      int x2 = r.right()  + 1;
      int y2 = r.bottom() + 1;

      bool success = fractalGenerator.zoom({x1, y1}, {x2, y2});
      if (!success) {
          QMessageBox::warning(&window, "Zoom Error", "The selected rectangle was invalid.");
          actionPanel->setInfoText("Zoom failed");
          return;
      }
      // Refresh the canvas
      canvas->updateImage(fractalGenerator.imageData());
      actionPanel->setInfoText("Done!");

      // We definitely have at least one prior scale now
      actionPanel->setZoomOutAvailable(true);
  });

  // 3) The "Zoom Out" button in FractalActionPanel
  QObject::connect(actionPanel, &FractalActionPanel::zoomOutRequest,
                   [&]() {
      if (!fractalGenerator.isPreviousScale()) {
          QMessageBox::information(&window, "No Zoom History", "No more zoom out is available.");
          return;
      }
      actionPanel->setInfoText("Zooming out...");

      fractalGenerator.setPreviousScale();
      canvas->updateImage(fractalGenerator.imageData());
      actionPanel->setInfoText("Done!");

      bool canZoomOut = fractalGenerator.isPreviousScale();
      actionPanel->setZoomOutAvailable(canZoomOut);
  });

  window.show();
  return app.exec();
}
