#ifndef FRACTALPARAMETERPANEL_HPP
#define FRACTALPARAMETERPANEL_HPP

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <type_traits>
#include <utility>
#include <vector>

#include "FractalAlgorithm.hpp"
#include "FractalGenerator.hpp"

class FractalParameterPanel : public QWidget {
  Q_OBJECT

 public:
  explicit FractalParameterPanel(QWidget* parent = nullptr);

  int getExponent() const;
  std::pair<QString, QString> getIncrement() const;
  std::pair<QString, QString> getRelaxation() const;
  std::pair<QString, QString> getStartValue() const;
  bool isPixStartEnabled() const;
  std::vector<std::pair<QString, QString>> getPolynomial() const;
  QString getSelectedFractalType() const;
  bool validateInputs() const;
  Configuration collectParameters() const;

 signals:
  void generateFractal();

 private slots:
  void onFractalTypeChanged(const QString& type);
  void onAddPolyClicked();
  void onClearPolyClicked();
  void onPixStartChanged(bool checked);
  void onDeleteSelectedPolyItem();

 private:
  void deallocateLayout(QLayout* layout);
  void clearParameterLayout();
  void showParametersFor(const QString& type);

  template <typename LabelT>
    requires std::is_same_v<std::remove_reference_t<LabelT>, QLabel>
  void addComplexField(LabelT&& label, QLineEdit*& realOut,
                       QLineEdit*& imagOut) {
    label.setText(label.text() + ":");
    parameterLayout->addWidget(&label);

    // Row for Re:
    QHBoxLayout* reLayout = new QHBoxLayout;
    QLabel* reLabel = new QLabel("Re:");
    if (realOut == nullptr) {
      realOut = new QLineEdit;
    }
    realOut->setValidator(new QDoubleValidator(realOut));
    reLabel->setObjectName(realOut->objectName());
    reLayout->addWidget(reLabel);
    reLayout->addWidget(realOut);
    parameterLayout->addLayout(reLayout);

    // Row for Im:
    QHBoxLayout* imLayout = new QHBoxLayout;
    QLabel* imLabel = new QLabel("Im:");
    if (imagOut == nullptr) {
      imagOut = new QLineEdit;
    }
    imagOut->setValidator(new QDoubleValidator(imagOut));
    imLabel->setObjectName(imagOut->objectName());
    imLayout->addWidget(imLabel);
    imLayout->addWidget(imagOut);
    parameterLayout->addLayout(imLayout);
  }

  QString formatPolyTerm(int exponent, const QString& re, const QString& im);

  QComboBox* fractalSelector;
  QVBoxLayout* mainLayout;
  QVBoxLayout* parameterLayout;

  QLineEdit* exponentInput = nullptr;
  QLineEdit *incReal = nullptr, *incImag = nullptr;
  QLineEdit *relaxReal = nullptr, *relaxImag = nullptr;
  QLineEdit *startReal = nullptr, *startImag = nullptr;
  QCheckBox* pixStartCheck = nullptr;

  QListWidget* polyList = nullptr;
  QLineEdit *polyReal = nullptr, *polyImag = nullptr;
  QPushButton* addPolyBtn = nullptr;
  QPushButton* clearPolyBtn = nullptr;

  QLineEdit* minRField = nullptr;
  QLineEdit* maxRField = nullptr;
  QLineEdit* minIField = nullptr;
  QLineEdit* maxIField = nullptr;
};

#endif  // FRACTALPARAMETERPANEL_HPP
