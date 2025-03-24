#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>

class ImageCanvas : public QWidget {
public:
    ImageCanvas(QWidget *parent = nullptr)
        : QWidget(parent), image(1080, 720, QImage::Format_RGB32)
    {
        setFixedSize(1080, 720);
        fillPixels();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.drawImage(0, 0, image);
    }

private:
    QImage image;

    void fillPixels() {
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                image.setPixel(x, y, qRgb(0, 0, (x + y) % 256)); // Blue gradient
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Main window
    QWidget window;
    window.setFixedSize(1230, 720); // 1080 + 150
    window.setWindowTitle("RGB Plane + UI Panel");

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Side panel (150px)
    QWidget *sidePanel = new QWidget;
    sidePanel->setFixedWidth(150);
    sidePanel->setStyleSheet("background-color: #eee;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);

    QRadioButton *cpuRadio = new QRadioButton("CPU");
    QRadioButton *gpuRadio = new QRadioButton("GPU");

    QButtonGroup *modeGroup = new QButtonGroup(sidePanel);
    modeGroup->addButton(cpuRadio);
    modeGroup->addButton(gpuRadio);
    cpuRadio->setChecked(true);

    sideLayout->addStretch();
    sideLayout->addWidget(cpuRadio, 0, Qt::AlignHCenter);
    sideLayout->addWidget(gpuRadio, 0, Qt::AlignHCenter);
    sideLayout->addStretch();

    // Image canvas (1080px)
    ImageCanvas *canvas = new ImageCanvas;

    // Add to layout
    mainLayout->addWidget(sidePanel);
    mainLayout->addWidget(canvas);

    window.show();
    return app.exec();
}
