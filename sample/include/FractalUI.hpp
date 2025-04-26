#ifndef FRACTALUI_HPP
#define FRACTALUI_HPP

#include <QWidget>

class FractalParameterPanel;
class FractalActionPanel;

class FractalUI : public QWidget {
  Q_OBJECT
 public:
  explicit FractalUI(QWidget* parent = nullptr);

  // Provide getters so we can access them from outside
  FractalParameterPanel* parameterPanel() const;
  FractalActionPanel* actionPanel() const;

 private:
  FractalParameterPanel* m_parameterPanel = nullptr;
  FractalActionPanel* m_actionPanel = nullptr;
};

#endif  // FRACTALUI_HPP
