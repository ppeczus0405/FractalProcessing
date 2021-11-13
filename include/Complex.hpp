#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include <iostream>

/**
 * @brief Handles complex numbers
 * 
 */
class Complex
{
    public:
        /**
         * @brief Construct a new Complex number
         * 
         */
        Complex() = default;
        
        /**
         * @brief Construct a new Complex number from real and imaginary value
         * 
         * @param real_v - real value
         * @param imaginary_v - imaginary value
         */
        Complex(long double real_v, long double imaginary_v);
        
        /**
         * @brief Copy constructor
         * 
         * @param toCopy - number to copy
         */
        Complex(const Complex & toCopy);
        
        /**
         * @brief Move constructor
         * 
         * @param toMove - number to move
         */
        Complex(Complex && toMove);



        /**
         * @brief Complex number equal to 0 + 0i
         * 
         */
        static const Complex ZERO;
        
        /**
         * @brief Complex number equal to 1 + 0i
         * 
         */
        static const Complex ONE;

        /**
         * @brief Power of complex number
         * 
         * @param c - complex number
         * @param exponent - any integer
         * @return c to the power of exponent
         */
        static Complex power(const Complex &c, int exponent);

        /**
         * @brief Complex conjugate
         * 
         * @param c - complex number
         * @return conjugate of c
         */
        static Complex conjugate(const Complex &c);

        /**
         * @brief Absolute value of complex number
         * 
         * @param c - complex number
         * @return absolute value of c
         */
        static long double absolute(const Complex &c);

        /**
         * @brief Absolute value of complex number to the power of 2
         * 
         * @param c - complex number
         * @return absolute value of c to the power of 2
         */
        static long double absolute_square(const Complex &c);

        /**
         * @brief Get the real value
         * 
         * @return real value of complex number
         */
        long double getReal() const noexcept;

        /**
         * @brief Get the imaginary value
         * 
         * @return imaginary value of complex number
         */
        long double getImaginary() const noexcept;

        /**
         * @brief Set the real value
         * 
         * @param value - setted real part of number
         */
        void setReal(long double value);
        
        /**
         * @brief Set the imaginary value
         * 
         * @param value - setted imaginary part of number
         */
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
        /**
         * @brief Real part of complex number
         * 
         */
        long double real = 0.0L;

        /**
         * @brief Imaginary part of complex number
         * 
         */
        long double imaginary = 0.0L;
};

#endif // COMPLEX_HPP