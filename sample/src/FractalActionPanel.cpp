#include "FractalActionPanel.hpp"

#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QIcon>

namespace {
    void setLabelColor(QWidget* widget, const QString& color) {
        widget->setStyleSheet(QString("color: %1;").arg(color));
    }
}

FractalActionPanel::FractalActionPanel(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Horizontal layout for "Generate" + "Zoom Out"
    QHBoxLayout* buttonRow = new QHBoxLayout;

    // Generate Button
    m_generateButton = new QPushButton("Generate", this);
    buttonRow->addWidget(m_generateButton);

    // Zoom Out Button, with a small icon
    m_zoomOutButton = new QPushButton("Zoom Out", this);
    m_zoomOutButton->setIcon(QIcon(":/icons/zoomout.svg")); // example resource path
    m_zoomOutButton->setIconSize(QSize(16, 16));
    buttonRow->addWidget(m_zoomOutButton);

    mainLayout->addLayout(buttonRow);

    // Info Label
    m_infoLabel = new QLabel("Ready", this);
    // Give it an object name so only this label can have special background styling
    m_infoLabel->setObjectName("infoLabel");
    mainLayout->addWidget(m_infoLabel);

    // Spacer at bottom
    mainLayout->addStretch(1);

    // Default to "cannot zoom out"
    setZoomOutAvailable(false);

    // Wiring signals
    connect(m_generateButton, &QPushButton::clicked,
            this, &FractalActionPanel::generateFractal);

    connect(m_zoomOutButton, &QPushButton::clicked,
            this, &FractalActionPanel::zoomOutRequest);
}

void FractalActionPanel::setInfoText(const QString &text)
{
    m_infoLabel->setText(text);
}

void FractalActionPanel::setZoomOutAvailable(bool available)
{
    // If you only want to recolor the text, do:
    if (available) {
        setLabelColor(m_zoomOutButton, "#f0f0f0");
    } else {
        setLabelColor(m_zoomOutButton, "#888");
    }

    // Alternatively, if you want to fully disable the button:
    // m_zoomOutButton->setEnabled(available);
}
