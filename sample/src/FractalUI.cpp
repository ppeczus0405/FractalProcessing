#include "FractalUI.hpp"
#include "FractalActionPanel.hpp"
#include "FractalParameterPanel.hpp"

#include <QVBoxLayout>

FractalUI::FractalUI(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(6, 6, 6, 6);
  layout->setSpacing(4);

  setFixedWidth(300);
  setStyleSheet(R"(
        /* Base widget styling */
        QWidget {
            background-color: #2e2e2e;
            color: #f0f0f0;
            font-size: 13px;
        }
        /* Inputs: QLineEdit, SpinBoxes, etc. */
        QLineEdit, QSpinBox, QDoubleSpinBox, QListWidget {
            background-color: #2e2e2e;
            color: #f0f0f0;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 4px;
        }
        /* Generic label styling */
        QLabel {
            color: #cccccc;
            background-color: #2e2e2e;
        }
        /* The special label with objectName="infoLabel" -> distinct background */
        #infoLabel {
            background-color: #2e2e2e;
            color: #f0f0f0;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 6px;
            margin-top: 4px;
        }
        /* CheckBox, PushButton normal states */
        QCheckBox, QPushButton {
            background-color: #333;
            color: #f0f0f0;
            border: 1px solid #444;
            border-radius: 4px;
            padding: 4px 6px;
        }
        QComboBox {
            background-color: #2e2e2e;
            border: 1px solid #444;
            min-height: 25px;
            border-radius: 4px;
            color: #f0f0f0;
            padding-left: 5px;
        }
        /* Styles the popup list itself */
        QComboBox QAbstractItemView {
            background-color: #2e2e2e;
            border: 1px solid #444;
            margin: 0;                /* remove any margin */
            padding: 0;               /* remove any padding */
            selection-background-color: #444;
            selection-color: #f0f0f0;
        }
        /* Button hover & press */
        QPushButton:hover {
            background-color: #444;
        }
        QPushButton:pressed {
            background-color: #555;
        }
        /* Disabled => gray text */
        QPushButton:disabled,
        QLineEdit:disabled,
        QSpinBox:disabled,
        QDoubleSpinBox:disabled,
        QListWidget:disabled,
        QComboBox:disabled {
            background-color: #333;
            color: #888;
            border: 1px solid #555;
        }
        /* RubberBand selection, optional */
        QRubberBand {
            border: 1px solid rgba(255, 255, 255, 0.4);
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");

  // 1) The parameter panel at the top
  m_parameterPanel = new FractalParameterPanel(this);
  layout->addWidget(m_parameterPanel);

  layout->addStretch(1);

  // 2) The action panel (Generate, Info label, Zoom out)
  m_actionPanel = new FractalActionPanel(this);
  layout->addWidget(m_actionPanel);
}

FractalParameterPanel* FractalUI::parameterPanel() const {
  return m_parameterPanel;
}

FractalActionPanel* FractalUI::actionPanel() const {
  return m_actionPanel;
}
