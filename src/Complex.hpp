#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include <iostream>

class Complex
{
    public:
        // Constructors
        Complex(double real_v, double imaginary_v);
        Complex(const Complex & toCopy);
        Complex(Complex && toMove);

        // Static useful functions
        static Complex power(const Complex &c, int exponent);
        static Complex conjugate(const Complex &c);
        static double absolute(const Complex &c);
        static double absolute_square(const Complex &c);

        // Getters
        double getReal() const noexcept;
        double getImaginary() const noexcept;

        // Setters
        void setReal(double value);
        void setImaginary(double value);

        // Copy / Move
        Complex & operator=(const Complex &c);
        Complex & operator=(Complex &&c);

        // Addition
        Complex operator+(const Complex &c);
        Complex & operator+=(const Complex &c);

        // Substraction
        Complex operator-(const Complex &c);
        Complex & operator-=(const Complex &c);

        // Multiplication
        Complex operator*(const Complex &c);
        Complex operator*(const double a); // Scalar
        Complex & operator*=(const double a); // Scalar
        Complex & operator*=(const Complex &c);

        // Division
        Complex operator/(const Complex &c);
        Complex operator/(const double a); // Scalar
        Complex & operator/=(const double a); // Scalar
        Complex & operator/=(const Complex &c);

        friend std::ostream & operator<<(std::ostream &stream, const Complex &c);
    private:
        double real, imaginary;

};

#endif // COMPLEX_HPP