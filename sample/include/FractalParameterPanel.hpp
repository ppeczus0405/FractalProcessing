#ifndef FRACTALPARAMETERPANEL_HPP
#define FRACTALPARAMETERPANEL_HPP

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <vector>
#include <utility>

class FractalParameterPanel : public QWidget {
    Q_OBJECT

public:
    explicit FractalParameterPanel(QWidget *parent = nullptr);

    int getExponent() const;
    std::pair<QString, QString> getIncrement() const;
    std::pair<QString, QString> getRelaxation() const;
    std::pair<QString, QString> getStartValue() const;
    bool isPixStartEnabled() const;
    std::vector<std::pair<QString, QString>> getPolynomial() const;
    QString getSelectedFractalType() const;

signals:
    void generateFractal();

private slots:
    void onFractalTypeChanged(const QString &type);
    void onAddPolyClicked();
    void onClearPolyClicked();
    void onPixStartChanged(bool checked);
    void onDeleteSelectedPolyItem();

private:
    void clearParameterLayout();
    void showParametersFor(const QString &type);
    void addComplexField(const QString &label, QLineEdit *&realOut, QLineEdit *&imagOut);
    QString formatPolyTerm(int exponent, const QString &re, const QString &im);

    QComboBox *fractalSelector;
    QVBoxLayout *mainLayout;
    QVBoxLayout *parameterLayout;

    QLineEdit *exponentInput = nullptr;
    QLineEdit *incReal = nullptr, *incImag = nullptr;
    QLineEdit *relaxReal = nullptr, *relaxImag = nullptr;
    QLineEdit *startReal = nullptr, *startImag = nullptr;
    QCheckBox *pixStartCheck = nullptr;

    QListWidget *polyList = nullptr;
    QLineEdit *polyReal = nullptr, *polyImag = nullptr;
    QPushButton *addPolyBtn = nullptr;
    QPushButton *clearPolyBtn = nullptr;

    QPushButton *generateButton;
};

#endif // FRACTALPARAMETERPANEL_HPP

