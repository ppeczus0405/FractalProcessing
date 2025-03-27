#include <qvalidator.h>
#include <QIntValidator>
#include <QMenu>
#include <QRegularExpression>

#include "FractalAlgorithm.hpp"
#include "FractalParameterPanel.hpp"

FractalParameterPanel::FractalParameterPanel(QWidget* parent)
    : QWidget(parent) {
  fractalSelector = new QComboBox;
  fractalSelector->addItems(
      {"Mandelbrot", "Multibrot", "Julia", "PolyJulia", "Newton", "Nova"});

  mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(6, 6, 6, 6);
  mainLayout->setSpacing(4);

  // === Fixed top section pinned to top ===
  QLabel* fractalLabel = new QLabel("Fractal Type:");
  mainLayout->addWidget(fractalLabel);
  mainLayout->addWidget(fractalSelector);

  // === Dynamic parameter section ===
  QWidget* paramContainer = new QWidget;
  parameterLayout = new QVBoxLayout(paramContainer);
  parameterLayout->setContentsMargins(0, 0, 0, 0);
  parameterLayout->setSpacing(6);
  mainLayout->addLayout(parameterLayout);
  mainLayout->addWidget(paramContainer);
  mainLayout->addStretch();

  generateButton = new QPushButton("Generate");
  mainLayout->addWidget(generateButton);

  connect(fractalSelector, &QComboBox::currentTextChanged, this,
          &FractalParameterPanel::onFractalTypeChanged);
  connect(generateButton, &QPushButton::clicked, this,
          &FractalParameterPanel::generateFractal);

  onFractalTypeChanged(fractalSelector->currentText());
}

Configuration FractalParameterPanel::collectParameters() const {
  Configuration config;

  auto fractal_type = getSelectedFractalType();
  if (fractal_type == "Mandelbrot") {
    config.fractalType = PekiProc::FractalAlgorithmType::MANDELBROT;
  } else if (fractal_type == "Multibrot") {
    config.fractalType = PekiProc::FractalAlgorithmType::MULTIBROT;
  } else if (fractal_type == "Julia") {
    config.fractalType = PekiProc::FractalAlgorithmType::JULIA;
  } else if (fractal_type == "PolyJulia") {
    config.fractalType = PekiProc::FractalAlgorithmType::POLYJULIA;
  } else if (fractal_type == "Newton") {
    config.fractalType = PekiProc::FractalAlgorithmType::NEWTON;
  } else if (fractal_type == "Nova") {
    config.fractalType = PekiProc::FractalAlgorithmType::NOVA;
  } else {
    assert(!"Invalid fractal type");
  }

  config.exponent = getExponent();

  auto increment = getIncrement();
  config.increment = {increment.first.toDouble(), increment.second.toDouble()};

  auto relaxation = getRelaxation();
  config.relaxation = {relaxation.first.toDouble(),
                       relaxation.second.toDouble()};

  auto startValue = getStartValue();
  config.startValue = {startValue.first.toDouble(),
                       startValue.second.toDouble()};

  config.usePixelStart = isPixStartEnabled();

  // Handle polynomial terms if applicable
  auto polynomialStrTerms = getPolynomial();
  for (const auto& term : polynomialStrTerms) {
    double realPart = term.first.toDouble();
    double imagPart = term.second.toDouble();
    config.polynomialTerms.emplace_back(realPart, imagPart);
  }

  return config;
}

void FractalParameterPanel::onFractalTypeChanged(const QString& type) {
  clearParameterLayout();

  if (type == "Multibrot" || type == "PolyJulia") {
    exponentInput = new QLineEdit;
    exponentInput->setValidator(new QIntValidator(exponentInput));
    parameterLayout->addWidget(new QLabel("Exponent:"));
    parameterLayout->addWidget(exponentInput);
  }

  if (type == "Julia" || type == "PolyJulia") {
    addComplexField(*(new QLabel("Increment")), incReal, incImag);
  }

  if (type == "Newton" || type == "Nova") {
    addComplexField(*(new QLabel("Relaxation")), relaxReal, relaxImag);

    addComplexField(*(new QLabel("Polynomial Term (Re/Im):")), polyReal,
                    polyImag);

    addPolyBtn = new QPushButton("Add Term");
    connect(addPolyBtn, &QPushButton::clicked, this,
            &FractalParameterPanel::onAddPolyClicked);
    parameterLayout->addWidget(addPolyBtn);

    clearPolyBtn = new QPushButton("Clear Polynomial");
    connect(clearPolyBtn, &QPushButton::clicked, this,
            &FractalParameterPanel::onClearPolyClicked);
    parameterLayout->addWidget(clearPolyBtn);

    polyList = new QListWidget;
    polyList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(polyList, &QListWidget::customContextMenuRequested,
            [&](const QPoint& pos) {
              QMenu menu;
              QAction* del = menu.addAction("Delete Term");
              if (menu.exec(polyList->mapToGlobal(pos)) == del) {
                onDeleteSelectedPolyItem();
              }
            });
    parameterLayout->addWidget(polyList);
  }

  if (type == "Nova") {
    pixStartCheck = new QCheckBox("Use Pixel Start");
    connect(pixStartCheck, &QCheckBox::toggled, this,
            &FractalParameterPanel::onPixStartChanged);
    parameterLayout->addWidget(pixStartCheck);

    QLabel* startLabel = new QLabel("Start Value");
    startLabel->setObjectName("startValGroup");
    startReal = new QLineEdit;
    startImag = new QLineEdit;
    startReal->setObjectName("startValGroup");
    startImag->setObjectName("startValGroup");
    addComplexField(*startLabel, startReal, startImag);
  }
}

void FractalParameterPanel::onAddPolyClicked() {
  if (!polyList || !polyReal || !polyImag)
    return;
  QString re = polyReal->text();
  QString im = polyImag->text();
  int exp = polyList->count();
  polyList->addItem(formatPolyTerm(exp, re, im));
}

void FractalParameterPanel::onClearPolyClicked() {
  if (polyList)
    polyList->clear();
}

void FractalParameterPanel::onDeleteSelectedPolyItem() {
  if (!polyList)
    return;
  auto items = polyList->selectedItems();
  for (auto* item : items) {
    delete polyList->takeItem(polyList->row(item));
  }

  // Renumber
  for (int i = 0; i < polyList->count(); ++i) {
    QString raw = polyList->item(i)->text();
    QRegularExpression re(R"(z\^\d+: \(([^,]+), ([^)]+)i\))");
    auto match = re.match(raw);
    if (match.hasMatch()) {
      QString real = match.captured(1);
      QString imag = match.captured(2);
      polyList->item(i)->setText(formatPolyTerm(i, real, imag));
    }
  }
}

QString FractalParameterPanel::formatPolyTerm(int exponent, const QString& re,
                                              const QString& im) {
  return QString("z^%1: (%2, %3i)").arg(exponent).arg(re).arg(im);
}

void FractalParameterPanel::onPixStartChanged(bool checked) {
  for (int i = 0; i < parameterLayout->count(); ++i) {
    QWidget* widget = parameterLayout->itemAt(i)->widget();
    if (widget && widget->objectName() == "startValGroup") {
      widget->setVisible(!checked);
    }
    QLayout* inner_layout = parameterLayout->itemAt(i)->layout();
    if (inner_layout != nullptr) {
      for (int j = 0; j < inner_layout->count(); ++j) {
        QWidget* inner_widget = inner_layout->itemAt(j)->widget();
        if (inner_widget && inner_widget->objectName() == "startValGroup") {
          inner_widget->setVisible(!checked);
        }
      }
    }
  }
}

bool FractalParameterPanel::validateInputs() const {
  auto isValid = [](QLineEdit* field) {
    if (!field)
      return true;
    return field->hasAcceptableInput();
  };

  bool startValid = pixStartCheck && !pixStartCheck->isChecked()
                        ? isValid(startReal) && isValid(startImag)
                        : true;
  bool polynomialValid = polyList ? getPolynomial().size() > 0 : true;

  return isValid(incReal) && isValid(incImag) && isValid(relaxReal) &&
         isValid(relaxImag) && isValid(exponentInput) && startValid &&
         polynomialValid;
}

int FractalParameterPanel::getExponent() const {
  return exponentInput ? exponentInput->text().toInt() : 1;
}

std::pair<QString, QString> FractalParameterPanel::getIncrement() const {
  return {incReal ? incReal->text() : "0", incImag ? incImag->text() : "0"};
}

std::pair<QString, QString> FractalParameterPanel::getRelaxation() const {
  return {relaxReal ? relaxReal->text() : "0",
          relaxImag ? relaxImag->text() : "0"};
}

std::pair<QString, QString> FractalParameterPanel::getStartValue() const {
  return {startReal ? startReal->text() : "0",
          startImag ? startImag->text() : "0"};
}

bool FractalParameterPanel::isPixStartEnabled() const {
  return pixStartCheck ? pixStartCheck->isChecked() : false;
}

std::vector<std::pair<QString, QString>> FractalParameterPanel::getPolynomial()
    const {
  std::vector<std::pair<QString, QString>> result;
  if (!polyList)
    return result;

  QRegularExpression re(R"(\(([^,]+), ([^)]+)i\))");
  for (int i = 0; i < polyList->count(); ++i) {
    QString text = polyList->item(i)->text();
    auto match = re.match(text);
    if (match.hasMatch()) {
      result.emplace_back(match.captured(1), match.captured(2));
    }
  }
  return result;
}

QString FractalParameterPanel::getSelectedFractalType() const {
  return fractalSelector->currentText();
}

void FractalParameterPanel::deallocateLayout(QLayout* layout) {
  if (!layout)
    return;

  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      widget->deleteLater();  // Schedule the widget for safe deletion
    } else if (QLayout* childLayout = item->layout()) {
      deallocateLayout(childLayout);  // Recursive deletion for nested layouts
    }
    delete item;  // Delete the layout item itself
  }
}

void FractalParameterPanel::clearParameterLayout() {
  deallocateLayout(parameterLayout);
  exponentInput = nullptr;
  incReal = incImag = nullptr;
  relaxReal = relaxImag = nullptr;
  startReal = startImag = nullptr;
  pixStartCheck = nullptr;
  polyReal = polyImag = nullptr;
  polyList = nullptr;
  addPolyBtn = nullptr;
  clearPolyBtn = nullptr;
}
