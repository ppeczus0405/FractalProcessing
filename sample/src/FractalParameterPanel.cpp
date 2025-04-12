#include <qcontainerfwd.h>
#include <qvalidator.h>
#include <QIntValidator>
#include <QMenu>
#include <QRegularExpression>
#include <charconv>

#include "Fractal.hpp"
#include "FractalAlgorithm.hpp"
#include "FractalGenerator.hpp"
#include "FractalParameterPanel.hpp"

namespace {
  double toDouble(const QString& qStr) {
    double value;
    std::string numText = qStr.toStdString();
    std::for_each(numText.begin(), numText.end(), [](char &c) {
      if(c == ',') c = '.';
    });
    auto [ptr, ec] = std::from_chars(numText.data(), numText.data() + numText.size(), value);
    if (ec != std::errc() || ptr != numText.data() + numText.size()) {
      std::cerr << "Conversion for " << numText << " failed. " << std::endl;
      assert(!"Dobule conversion error");
    }
    return value;
  }
}

FractalParameterPanel::FractalParameterPanel(QWidget* parent)
    : QWidget(parent) {
  fractalSelector = new QComboBox;
  fractalSelector->addItems(
      {"Mandelbrot", "Multibrot", "Julia", "PolyJulia", "Newton", "Nova"});
  // fractalSelector->setEditable(true);

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
  {
    // Create two horizontal layouts: one for the X range, one for the Y range
    QHBoxLayout* xRangeLayout = new QHBoxLayout;
    QLabel* xLabel = new QLabel("X:");
    xLabel->setFixedWidth(15);
    minRField = new QLineEdit;
    maxRField = new QLineEdit;

    // You can optionally add a QDoubleValidator to each field
    // so only double values are accepted
    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    minRField->setValidator(doubleValidator);
    maxRField->setValidator(doubleValidator);

    xRangeLayout->addWidget(xLabel);
    xRangeLayout->addWidget(minRField);
    xRangeLayout->addSpacing(5);
    xRangeLayout->addWidget(maxRField);

    QHBoxLayout* yRangeLayout = new QHBoxLayout;
    QLabel* yLabel = new QLabel("Y:");
    yLabel->setFixedWidth(15);
    minIField = new QLineEdit;
    maxIField = new QLineEdit;

    minIField->setValidator(doubleValidator);
    maxIField->setValidator(doubleValidator);

    yRangeLayout->addWidget(yLabel);
    yRangeLayout->addWidget(minIField);
    yRangeLayout->addSpacing(5);
    yRangeLayout->addWidget(maxIField);

    // Put them in a small container or directly into mainLayout
    mainLayout->addSpacing(5);
    mainLayout->addLayout(xRangeLayout);
    mainLayout->addLayout(yRangeLayout);
  }

  connect(fractalSelector, &QComboBox::currentTextChanged, this,
          &FractalParameterPanel::onFractalTypeChanged);

  onFractalTypeChanged(fractalSelector->currentText());
}

Configuration FractalParameterPanel::collectParameters() const {
  Configuration config;

  config.fractalType = FractalGenerator::fractalStringToType(getSelectedFractalType().toStdString());

  config.scaleParams = PekiProc::Dim{toDouble(minRField->text()),
                                     toDouble(maxRField->text()),
                                     toDouble(minIField->text()),
                                     toDouble(maxIField->text())
  };

  config.exponent = getExponent();

  auto increment = getIncrement();
  config.increment = {toDouble(increment.first), toDouble(increment.second)};

  auto relaxation = getRelaxation();
  config.relaxation = {toDouble(relaxation.first),
                       toDouble(relaxation.second)};

  auto startValue = getStartValue();
  config.startValue = {toDouble(startValue.first),
                       toDouble(startValue.second)};

  config.usePixelStart = isPixStartEnabled();

  // Handle polynomial terms if applicable
  auto polynomialStrTerms = getPolynomial();
  for (const auto& term : polynomialStrTerms) {
    double realPart = toDouble(term.first);
    double imagPart = toDouble(term.second);
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

    // Trigger default - checked.
    pixStartCheck->setChecked(false);
    pixStartCheck->toggle();
  }

    // =====================
    //  SET DEFAULT VALUES
    // =====================
    if (type == "Mandelbrot") {
        // No exponent or increment to set
        // Typically nothing extra to set for the classic Mandelbrot
    }
    else if (type == "Multibrot") {
        // Classic exponent = 3 (z^3 + c).  Feel free to change as you like.
        if (exponentInput) exponentInput->setText("3");
    }
    else if (type == "Julia") {
        // A common “classic” Julia is c = -0.7 + 0.27015i
        if (incReal) incReal->setText("-0,7");
        if (incImag) incImag->setText("0,27015");
    }
    else if (type == "PolyJulia") {
        // Custom one
        if (exponentInput) exponentInput->setText("5");
        if (incReal) incReal->setText("-0,70176");
        if (incImag) incImag->setText("-0,3842");
    }
    else if (type == "Newton") {
        // A classic polynomial is z^3 - 1
        // That means 4 polynomial terms: [1,0,0,-1]
        // Use onAddPolyClicked() to add them to polyList.
        // Relaxation often set to 1
        if (relaxReal) relaxReal->setText("1");
        if (relaxImag) relaxImag->setText("0");

        // Start by clearing any leftover list items
        onClearPolyClicked();

        // Add the polynomial terms for z^3 - 1
        // (lowest exponent => first item we add)
        polyReal->setText("1");
        polyImag->setText("0");
        onAddPolyClicked(); // => z^0 coefficient

        polyReal->setText("0");
        polyImag->setText("0");
        onAddPolyClicked(); // => z^1 coefficient

        polyReal->setText("0");
        polyImag->setText("0");
        onAddPolyClicked(); // => z^2 coefficient

        polyReal->setText("-1");
        polyImag->setText("0");
        onAddPolyClicked(); // => z^3 coefficient
    }
    else if (type == "Nova") {
        // Similar polynomial for Nova: z^3 - 1
        // Relaxation=1, StartValue=1+0i, or any typical values
        if (relaxReal) relaxReal->setText("1");
        if (relaxImag) relaxImag->setText("0");

        // Usually we do not rely on pixel start:
        if (pixStartCheck) pixStartCheck->setChecked(false);

        if (startReal) startReal->setText("1");
        if (startImag) startImag->setText("0");

        // Clear and add polynomial z^3 - 1
        onClearPolyClicked();

        polyReal->setText("1");
        polyImag->setText("0");
        onAddPolyClicked();

        polyReal->setText("0");
        polyImag->setText("0");
        onAddPolyClicked();

        polyReal->setText("0");
        polyImag->setText("0");
        onAddPolyClicked();

        polyReal->setText("-1");
        polyImag->setText("0");
        onAddPolyClicked();
    }


  const auto [minR, maxR, minI, maxI] = PekiProc::Fractal::getDefaultDimension(FractalGenerator::fractalStringToType(type.toStdString()));
  minRField->setText(QString(std::to_string(minR).data()));
  maxRField->setText(QString(std::to_string(maxR).data()));
  minIField->setText(QString(std::to_string(minI).data()));
  maxIField->setText(QString(std::to_string(maxI).data()));
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

  bool scaleValid = minRField && !minRField->text().isEmpty() &&
                    maxRField && !maxRField->text().isEmpty() &&
                    minIField && !minIField->text().isEmpty() &&
                    maxIField && !maxIField->text().isEmpty();

  return isValid(incReal) && isValid(incImag) && isValid(relaxReal) &&
         isValid(relaxImag) && isValid(exponentInput) && startValid &&
         polynomialValid && scaleValid;
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
  std::reverse(result.begin(), result.end());
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
