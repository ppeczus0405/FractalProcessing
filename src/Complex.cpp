#include "Complex.hpp"
#include "CompareDoubles.hpp"
#include <utility>
#include <cmath>

const Complex Complex::ZERO(0.0L, 0.0L);
const Complex Complex::ONE(1.0L, 0.0L);

Complex::Complex(long double real_v, long double imaginary_v) : real(real_v), imaginary(imaginary_v) { }

Complex::Complex(const Complex & toCopy){
    *this = toCopy;
}

Complex::Complex(Complex && toMove){
    *this = std::move(toMove);
}

Complex Complex::power(const Complex &c, int exponent){
    if(exponent == 0){
        return Complex::ONE;
    }
    bool neg_exp = exponent < 0;
    exponent = abs(exponent);
    Complex to_multiply(c);
    Complex result = Complex::ONE;
    while(exponent > 0){
        if(exponent & 1){
            result *= to_multiply;
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

long double Complex::absolute(const Complex &c){
    return sqrt(Complex::absolute_square(c));
}

long double Complex::absolute_square(const Complex &c){
    return c.real * c.real + c.imaginary * c.imaginary;
}

long double Complex::getReal() const noexcept{
    return real;
}

long double Complex::getImaginary() const noexcept{
    return imaginary;
}

void Complex::setReal(long double value){
    real = value;
}

void Complex::setImaginary(long double value){
    imaginary = value;
}

Complex Complex::operator-(){
    return Complex(-this->real, -this->imaginary);
}

const Complex Complex::operator-() const{
    return Complex(-this->real, -this->imaginary);
}

bool Complex::operator==(const Complex &c){
    return CompareDoubles::isEqual(this->real, c.real) && 
           CompareDoubles::isEqual(this->imaginary, c.imaginary);
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
    long double real_result = this->real * c.real - this->imaginary * c.imaginary;
    long double imaginary_result = this->real * c.imaginary + this->imaginary * c.real; 
    return Complex(real_result, imaginary_result);
}

Complex Complex::operator*(const long double a){
    return Complex(a * this->real, a * this->imaginary);
}

Complex & Complex::operator*=(const long double a){
    return *this = *this * a;
}

Complex & Complex::operator*=(const Complex &c){
    return *this = *this * c;
}

Complex Complex::operator/(const Complex &c){
    bool realCompare = CompareDoubles::isEqual(c.real, 0.0L);
    bool imagCompare = CompareDoubles::isEqual(c.imaginary, 0.0L);
    if(realCompare && imagCompare){
        throw std::invalid_argument("Cannot divide by 0");
    }
    else if(realCompare && !imagCompare){
        return Complex(this->imaginary / c.imaginary, -this->real / c.imaginary);
    }
    else if(!realCompare && imagCompare){
        return *this / c.real;
    }
    long double real_result = this->real * c.real + this->imaginary * c.imaginary;
    long double imag_result = this->imaginary * c.real - this->real * c.imaginary;
    long double abs_square = Complex::absolute_square(c);
    return Complex(real_result / abs_square, imag_result / abs_square);
}

Complex Complex::operator/(const long double a){
    if(CompareDoubles::isEqual(a, 0.0L)){
        throw std::invalid_argument("Cannot divide by 0");
    }
    return Complex(this->real / a, this->imaginary / a);
}

Complex & Complex::operator/=(const long double a){
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