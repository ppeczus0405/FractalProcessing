#include "Complex.hpp"
#include "CompareDoubles.hpp"
#include <utility>
#include <cmath>

Complex::Complex(double real_v, double imaginary_v) : real(real_v), imaginary(imaginary_v) { }

Complex::Complex(const Complex & toCopy){
    *this = toCopy;
}

Complex::Complex(Complex && toMove){
    *this = std::move(toMove);
}

Complex Complex::power(const Complex &c, int exponent){
    if(exponent == 0){
        return Complex(1.0, 0.0);
    }
    bool neg_exp = exponent < 0;
    exponent = abs(exponent);
    Complex to_multiply(c);
    Complex result(1.0, 0.0);
    while(exponent > 0){
        if(exponent & 1){
            result *= to_multiply;
            std::cout << "EXP: " << to_multiply << std::endl;
        }
        exponent >>= 1;
        to_multiply *= to_multiply;
    }
    if(neg_exp){
        result = Complex::conjugate(result) / Complex::absolute_square(result);
    }
    return result;
}

Complex Complex::conjugate(const Complex &c){
    return Complex(c.real, -c.imaginary);
}

double Complex::absolute(const Complex &c){
    return sqrt(Complex::absolute_square(c));
}

double Complex::absolute_square(const Complex &c){
    return c.real * c.real + c.imaginary * c.imaginary;
}

double Complex::getReal() const noexcept{
    return real;
}

double Complex::getImaginary() const noexcept{
    return imaginary;
}

void Complex::setReal(double value){
    real = value;
}

void Complex::setImaginary(double value){
    imaginary = value;
}

Complex & Complex::operator=(const Complex &c){
    this->real = c.real;
    this->imaginary = c.imaginary;
    return *this;
}

Complex & Complex::operator=(Complex &&c){
    return *this = c;
}

Complex Complex::operator+(const Complex &c){
    return Complex(this->real + c.real, this->imaginary + c.imaginary);
}

Complex & Complex::operator+=(const Complex &c){
    return *this = *this + c;
}

Complex Complex::operator-(const Complex &c){
    return Complex(this->real - c.real, this->imaginary - c.imaginary);
}

Complex & Complex::operator-=(const Complex &c){
    return *this = *this - c;
}

Complex Complex::operator*(const Complex &c){
    double real_result = this->real * c.real - this->imaginary * c.imaginary;
    double imaginary_result = this->real * c.imaginary + this->imaginary * c.real; 
    return Complex(real_result, imaginary_result);
}

Complex Complex::operator*(const double a){
    return Complex(a * this->real, a * this->imaginary);
}

Complex & Complex::operator*=(const double a){
    return *this = *this * a;
}

Complex & Complex::operator*=(const Complex &c){
    return *this = *this * c;
}

Complex Complex::operator/(const Complex &c){
    bool realCompare = CompareDoubles::isEqual(c.real, 0.0f);
    bool imagCompare = CompareDoubles::isEqual(c.imaginary, 0.0f);
    if(realCompare && imagCompare){
        throw new std::invalid_argument("Cannot divide by 0");
    }
    else if(realCompare && !imagCompare){
        return Complex(this->imaginary / c.imaginary, -this->real / c.imaginary);
    }
    else if(!realCompare && imagCompare){
        return *this / c.real;
    }
    double real_result = this->real * c.real + this->imaginary * c.imaginary;
    double imag_result = this->imaginary * c.real - this->real * c.imaginary;
    double abs_square = Complex::absolute_square(c);
    return Complex(real_result / abs_square, imag_result / abs_square);
}

Complex Complex::operator/(const double a){
    if(CompareDoubles::isEqual(a, 0.0f)){
        throw new std::invalid_argument("Cannot divide by 0");
    }
    return Complex(this->real / a, this->imaginary / a);
}

Complex & Complex::operator/=(const double a){
    return *this = *this / a;
}

Complex & Complex::operator/=(const Complex &c){
    return *this = *this / c;
}

std::ostream & operator<<(std::ostream &stream, const Complex &c){
    stream << c.real;
    if(c.imaginary < 0){
        stream << " - ";
    }
    else{
        stream << " + ";
    }
    stream << fabs(c.imaginary) << "i";
    return stream;
}