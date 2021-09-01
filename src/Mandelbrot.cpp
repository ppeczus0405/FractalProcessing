#include "Mandelbrot.hpp"

int Mandelbrot::MAX_ITERATIONS = 1000;

int Mandelbrot::getIterations(Complex p)
{
    int iterations = 0;
    Complex z = p;
    while(iterations < MAX_ITERATIONS && Complex::absolute(z) < 2.0f){
        z = Complex::power(z, 2) + p;
        iterations++;
    }
    return iterations;
}

int Mandelbrot::getMaxIterations(){
    return MAX_ITERATIONS;
}

void Mandelbrot::setMaxIterations(int iterNumber)
{
    // Value of MAX_ITERATIONS is restricted due to performance
    // Value greater than 10^7 does not change much
    // Therefore MAX_ITERATIONS has integer value from set {1...10^7}
    if(iterNumber >= 1 && iterNumber <= 1e7){
        MAX_ITERATIONS = iterNumber;
    }
}