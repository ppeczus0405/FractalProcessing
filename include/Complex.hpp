#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include <iostream>
 
class Complex
{
    public:
        Complex() = default;
        Complex(long double real_v, long double imaginary_v);
        Complex(const Complex & toCopy);
        Complex(Complex && toMove);
 
        static const Complex ZERO;
        static const Complex ONE;
 
        static Complex power(const Complex &c, int exponent);
        static Complex conjugate(const Complex &c);
        static long double absolute(const Complex &c);
        static long double absolute_square(const Complex &c);
 
        long double getReal() const noexcept;
        long double getImaginary() const noexcept;
 
        void setReal(long double value);
        void setImaginary(long double value);
 
        // Overloaded operators
        bool operator==(const Complex &c);
 
        Complex & operator=(const Complex &c);
        Complex & operator=(Complex &&c);
 
        Complex operator-();
        const Complex operator-() const;
 
        // Addition
        Complex operator+(const Complex &c);
        Complex & operator+=(const Complex &c);
 
        // Substraction
        Complex operator-(const Complex &c);
        Complex & operator-=(const Complex &c);
 
        // Multiplication
        Complex operator*(const Complex &c);
        Complex operator*(const long double a); // Scalar
        Complex & operator*=(const long double a); // Scalar
        Complex & operator*=(const Complex &c);
 
        // Division
        Complex operator/(const Complex &c);
        Complex operator/(const long double a); // Scalar
        Complex & operator/=(const long double a); // Scalar
        Complex & operator/=(const Complex &c);
 
        friend std::ostream & operator<<(std::ostream &stream, const Complex &c);
    private:
        long double real = 0.0L;
        long double imaginary = 0.0L;
};
 
#endif // COMPLEX_HPP