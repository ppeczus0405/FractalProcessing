#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QLabel>

#include "ImageCanvas.hpp"
#include "FractalParameterPanel.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(1230, 720);
    window.setWindowTitle("Fractal Generator");

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === Side Panel (150px) ===
    QWidget *sidePanel = new QWidget;
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

    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);
    FractalParameterPanel *parameterPanel = new FractalParameterPanel;

    sideLayout->addWidget(parameterPanel);

    // === Image Canvas ===
    ImageCanvas *canvas = new ImageCanvas;

    // === Layout Composition ===
    mainLayout->addWidget(sidePanel);
    mainLayout->addWidget(canvas);

    // === Signal connection (placeholder) ===
    QObject::connect(parameterPanel, &FractalParameterPanel::generateFractal, [&]() {
        // Placeholder for fractal generation logic using parameterPanel->get*() methods
        qDebug("Generate button clicked!");
    });

    window.show();
    return app.exec();
}

