#include "FractalParameterPanel.hpp"
#include <QMenu>
#include <QRegularExpression>

FractalParameterPanel::FractalParameterPanel(QWidget *parent)
    : QWidget(parent)
{
    fractalSelector = new QComboBox;
    fractalSelector->addItems({
        "Mandelbrot", "Multibrot", "Julia", "PolyJulia", "Newton", "Nova"
    });

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(4);

    // === Fixed top section pinned to top ===
    QLabel *fractalLabel = new QLabel("Fractal Type:");
    mainLayout->addWidget(fractalLabel);
    mainLayout->addWidget(fractalSelector);


    // === Dynamic parameter section ===
    QWidget *paramContainer = new QWidget;
    parameterLayout = new QVBoxLayout(paramContainer);
    parameterLayout->setContentsMargins(0, 0, 0, 0);
    parameterLayout->setSpacing(6);
    mainLayout->addLayout(parameterLayout);
    mainLayout->addWidget(paramContainer);
    mainLayout->addStretch();

    generateButton = new QPushButton("Generate");
    mainLayout->addWidget(generateButton);


    connect(fractalSelector, &QComboBox::currentTextChanged,
            this, &FractalParameterPanel::onFractalTypeChanged);
    connect(generateButton, &QPushButton::clicked,
            this, &FractalParameterPanel::generateFractal);

    onFractalTypeChanged(fractalSelector->currentText());
}

void FractalParameterPanel::clearParameterLayout() {
    QLayoutItem *child;
    while ((child = parameterLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

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

void FractalParameterPanel::onFractalTypeChanged(const QString &type) {
    clearParameterLayout();

    if (type == "Multibrot" || type == "PolyJulia") {
        exponentInput = new QLineEdit;
        parameterLayout->addWidget(new QLabel("Exponent:"));
        parameterLayout->addWidget(exponentInput);
    }

    if (type == "Julia" || type == "PolyJulia") {
        addComplexField("Increment", incReal, incImag);
    }

    if (type == "Newton" || type == "Nova") {
        addComplexField("Relaxation", relaxReal, relaxImag);

        parameterLayout->addWidget(new QLabel("Polynomial Term (Re/Im):"));
        polyReal = new QLineEdit;
        polyImag = new QLineEdit;
        parameterLayout->addWidget(polyReal);
        parameterLayout->addWidget(polyImag);

        addPolyBtn = new QPushButton("Add Term");
        connect(addPolyBtn, &QPushButton::clicked, this, &FractalParameterPanel::onAddPolyClicked);
        parameterLayout->addWidget(addPolyBtn);

        clearPolyBtn = new QPushButton("Clear Polynomial");
        connect(clearPolyBtn, &QPushButton::clicked, this, &FractalParameterPanel::onClearPolyClicked);
        parameterLayout->addWidget(clearPolyBtn);

        polyList = new QListWidget;
        polyList->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(polyList, &QListWidget::customContextMenuRequested, [&](const QPoint &pos) {
            QMenu menu;
            QAction *del = menu.addAction("Delete Term");
            if (menu.exec(polyList->mapToGlobal(pos)) == del) {
                onDeleteSelectedPolyItem();
            }
        });
        parameterLayout->addWidget(polyList);
    }

    if (type == "Nova") {
        pixStartCheck = new QCheckBox("Use Pixel Start");
        connect(pixStartCheck, &QCheckBox::toggled, this, &FractalParameterPanel::onPixStartChanged);
        parameterLayout->addWidget(pixStartCheck);

        QLabel *startLabel = new QLabel("Start Value:");
        startLabel->setObjectName("startValLabel");
        parameterLayout->addWidget(startLabel);

        startReal = new QLineEdit;
        startImag = new QLineEdit;
        startReal->setObjectName("startValWidget");
        startImag->setObjectName("startValWidget");
        parameterLayout->addWidget(startReal);
        parameterLayout->addWidget(startImag);
    }
}

void FractalParameterPanel::addComplexField(const QString &label, QLineEdit *&realOut, QLineEdit *&imagOut) {
    parameterLayout->addWidget(new QLabel(label + ":"));

    // Row for Re:
    QHBoxLayout *reLayout = new QHBoxLayout;
    QLabel *reLabel = new QLabel("Re:");
    realOut = new QLineEdit;
    reLayout->addWidget(reLabel);
    reLayout->addWidget(realOut);
    parameterLayout->addLayout(reLayout);

    // Row for Im:
    QHBoxLayout *imLayout = new QHBoxLayout;
    QLabel *imLabel = new QLabel("Im:");
    imagOut = new QLineEdit;
    imLayout->addWidget(imLabel);
    imLayout->addWidget(imagOut);
    parameterLayout->addLayout(imLayout);
}

void FractalParameterPanel::onAddPolyClicked() {
    if (!polyList || !polyReal || !polyImag) return;
    QString re = polyReal->text();
    QString im = polyImag->text();
    int exp = polyList->count();
    polyList->addItem(formatPolyTerm(exp, re, im));
}

void FractalParameterPanel::onClearPolyClicked() {
    if (polyList) polyList->clear();
}

void FractalParameterPanel::onDeleteSelectedPolyItem() {
    if (!polyList) return;
    auto items = polyList->selectedItems();
    for (auto *item : items) {
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

QString FractalParameterPanel::formatPolyTerm(int exponent, const QString &re, const QString &im) {
    return QString("z^%1: (%2, %3i)").arg(exponent).arg(re).arg(im);
}

void FractalParameterPanel::onPixStartChanged(bool checked) {
    for (int i = 0; i < parameterLayout->count(); ++i) {
        QWidget *widget = parameterLayout->itemAt(i)->widget();
        if (widget && widget->objectName() == "startValWidget") {
            widget->setVisible(!checked);
        }
    }
}

int FractalParameterPanel::getExponent() const {
    return exponentInput ? exponentInput->text().toInt() : 1;
}

std::pair<QString, QString> FractalParameterPanel::getIncrement() const {
    return {incReal ? incReal->text() : "0", incImag ? incImag->text() : "0"};
}

std::pair<QString, QString> FractalParameterPanel::getRelaxation() const {
    return {relaxReal ? relaxReal->text() : "0", relaxImag ? relaxImag->text() : "0"};
}

std::pair<QString, QString> FractalParameterPanel::getStartValue() const {
    return {startReal ? startReal->text() : "0", startImag ? startImag->text() : "0"};
}

bool FractalParameterPanel::isPixStartEnabled() const {
    return pixStartCheck ? pixStartCheck->isChecked() : false;
}

std::vector<std::pair<QString, QString>> FractalParameterPanel::getPolynomial() const {
    std::vector<std::pair<QString, QString>> result;
    if (!polyList) return result;

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

