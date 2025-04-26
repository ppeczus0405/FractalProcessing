#include <cmath>
#include "Complex.hpp"

namespace PekiProc {

CUDA_HD Complex::Complex() : real(0.0), imaginary(0.0) {}

CUDA_HD Complex::Complex(double real_v, double imaginary_v)
    : real(real_v), imaginary(imaginary_v) {}

// --- Math functions ---
CUDA_HD double Complex::absolute(const Complex& c) {
  return sqrt(c.real * c.real + c.imaginary * c.imaginary);
}

CUDA_HD double Complex::absolute_square(const Complex& c) {
  return c.real * c.real + c.imaginary * c.imaginary;
}

CUDA_HD Complex Complex::conjugate(const Complex& c) {
  return Complex(c.real, -c.imaginary);
}

CUDA_HD Complex Complex::power(const Complex& c, int exponent) {
  Complex result = Complex::ONE();
  Complex base = c;
  bool negative = exponent < 0;
  exponent = abs(exponent);

  while (exponent) {
    if (exponent & 1)
      result *= base;
    base *= base;
    exponent >>= 1;
  }

  if (negative) {
    double abs_sq = absolute_square(result);
    result = conjugate(result) / abs_sq;
  }
  return result;
}

// --- Operators ---
CUDA_HD Complex Complex::operator+(const Complex& other) const {
  return Complex(real + other.real, imaginary + other.imaginary);
}

CUDA_HD Complex& Complex::operator+=(const Complex& other) {
  real += other.real;
  imaginary += other.imaginary;
  return *this;
}

CUDA_HD Complex Complex::operator-(const Complex& other) const {
  return Complex(real - other.real, imaginary - other.imaginary);
}

CUDA_HD Complex& Complex::operator-=(const Complex& other) {
  real -= other.real;
  imaginary -= other.imaginary;
  return *this;
}

CUDA_HD Complex Complex::operator*(const Complex& other) const {
  return Complex(real * other.real - imaginary * other.imaginary,
                 real * other.imaginary + imaginary * other.real);
}

CUDA_HD Complex& Complex::operator*=(const Complex& other) {
  *this = *this * other;
  return *this;
}

CUDA_HD Complex Complex::operator*(double scalar) const {
  return Complex(real * scalar, imaginary * scalar);
}

CUDA_HD Complex& Complex::operator*=(double scalar) {
  real *= scalar;
  imaginary *= scalar;
  return *this;
}

CUDA_HD Complex Complex::operator/(const Complex& other) const {
  double denom = other.real * other.real + other.imaginary * other.imaginary;
  return Complex((real * other.real + imaginary * other.imaginary) / denom,
                 (imaginary * other.real - real * other.imaginary) / denom);
}

CUDA_HD Complex Complex::operator/(double scalar) const {
  return Complex(real / scalar, imaginary / scalar);
}

CUDA_HD Complex& Complex::operator/=(const Complex& other) {
  *this = *this / other;
  return *this;
}

CUDA_HD Complex& Complex::operator/=(double scalar) {
  real /= scalar;
  imaginary /= scalar;
  return *this;
}

CUDA_HD bool Complex::operator==(const Complex& other) const {
  return (real == other.real && imaginary == other.imaginary);
}

CUDA_HD Complex Complex::operator-() const {
  return Complex(-real, -imaginary);
}

}  // namespace PekiProc

std::ostream& operator<<(std::ostream& stream, const PekiProc::Complex& c) {
  stream << c.real;
  if (c.imaginary < 0) {
    stream << " - ";
  } else {
    stream << " + ";
  }
  stream << fabs(c.imaginary) << "i";
  return stream;
}
