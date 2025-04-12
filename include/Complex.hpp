#ifndef PEKI_COMPLEX_HPP
#define PEKI_COMPLEX_HPP
#include <iostream>

namespace PekiProc {
class Complex;
}

// We need to override global operator so forward declare it before.
std::ostream& operator<<(std::ostream& stream, const PekiProc::Complex& c);

namespace PekiProc {

class Complex {
 public:
  Complex() = default;
  Complex(double real_v, double imaginary_v);
  Complex(const Complex& toCopy);
  Complex(Complex&& toMove);

  static const Complex ZERO;
  static const Complex ONE;

  static Complex power(const Complex& c, int exponent);
  static Complex conjugate(const Complex& c);
  static double absolute(const Complex& c);
  static double absolute_square(const Complex& c);

  double getReal() const noexcept;
  double getImaginary() const noexcept;

  void setReal(double value);
  void setImaginary(double value);

  // Overloaded operators
  bool operator==(const Complex& c);

  Complex& operator=(const Complex& c);
  Complex& operator=(Complex&& c);

  Complex operator-();
  const Complex operator-() const;

  // Addition
  Complex operator+(const Complex& c);
  Complex& operator+=(const Complex& c);

  // Substraction
  Complex operator-(const Complex& c);
  Complex& operator-=(const Complex& c);

  // Multiplication
  Complex operator*(const Complex& c);
  Complex operator*(const double a);    // Scalar
  Complex& operator*=(const double a);  // Scalar
  Complex& operator*=(const Complex& c);

  // Division
  Complex operator/(const Complex& c);
  Complex operator/(const double a);    // Scalar
  Complex& operator/=(const double a);  // Scalar
  Complex& operator/=(const Complex& c);

  friend std::ostream& ::operator<<(std::ostream& stream, const Complex& c);

 private:
  double real = 0.0;
  double imaginary = 0.0;
};

}  // namespace PekiProc

#endif  // PEKI_COMPLEX_HPP
