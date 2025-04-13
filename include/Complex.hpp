#ifndef PEKI_COMPLEX_HPP
#define PEKI_COMPLEX_HPP

#include <iostream>
#include "CudaCompat.hpp"


namespace PekiProc {

class Complex {
 public:
  double real = 0.0;
  double imaginary = 0.0;

  CUDA_HD Complex();
  CUDA_HD Complex(double real_v, double imaginary_v);

  CUDA_HD static Complex ZERO() { return Complex(0.0, 0.0); }
  CUDA_HD static Complex ONE() { return Complex(1.0, 0.0); }

  CUDA_HD static Complex power(const Complex& c, int exponent);
  CUDA_HD static Complex conjugate(const Complex& c);
  CUDA_HD static double absolute(const Complex& c);
  CUDA_HD static double absolute_square(const Complex& c);

  // Overloaded operators
  CUDA_HD bool operator==(const Complex& c) const;

  CUDA_HD Complex operator-() const;

  // Addition
  CUDA_HD Complex operator+(const Complex& c) const;
  CUDA_HD Complex& operator+=(const Complex& c);

  // Substraction
  CUDA_HD Complex operator-(const Complex& c) const;
  CUDA_HD Complex& operator-=(const Complex& c);

  // Multiplication
  CUDA_HD Complex operator*(const Complex& c) const;
  CUDA_HD Complex operator*(const double a) const;    // Scalar
  CUDA_HD Complex& operator*=(const double a);  // Scalar
  CUDA_HD Complex& operator*=(const Complex& c);

  // Division
  CUDA_HD Complex operator/(const Complex& c) const;
  CUDA_HD Complex operator/(const double a) const;    // Scalar
  CUDA_HD Complex& operator/=(const double a);  // Scalar
  CUDA_HD Complex& operator/=(const Complex& c);
};

}  // namespace PekiProc

std::ostream& operator<<(std::ostream& stream, const PekiProc::Complex& c);

#endif  // PEKI_COMPLEX_HPP
