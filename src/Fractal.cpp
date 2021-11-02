#include "Fractal.hpp"

// Dimension of the fractal plane (minReal, maxReal, minImag, maxImag)
using Dim = tuple <long double, long double, long double, long double>;

const Dim DefaultMandelbrotDim = Dim(-2.2L, 1.0L, -1.2L, 1.2L);
const Dim DefaultMultibrotDim  = Dim(-2.2L, 2.2L, -1.65L, 1.65L);
const Dim DefaultJuliaDim      = Dim(-2.0L, 2.0L, -1.5L, 1.5L);
const Dim DefaultNewtonDim     = Dim(-2.8L, 2.8L, -2.1L, 2.1L);
const Dim DefaultPolyJuliaDim  = DefaultMultibrotDim;
const Dim DefaultNovaDim       = DefaultJuliaDim;

// Fractal
Fractal::Fractal(int width, int height) : Image(width, height) { }

template <typename T>
void Fractal::setGradient(T&& gradient){

}

void Fractal::resize(int width, int height){

}

void Fractal::setIterations(int iters){

}

void Fractal::setAlgorithm(unique_ptr<FractalAlgorithm> alg){

}

void Fractal::setScale(long double minR, long double maxR, long double minI, long double maxI){

}

void Fractal::zoom(long long times){

}

void Fractal::unzoom(long long times){
    
}

// FractalBuilder
FractalBuilder::FractalBuilder(int width, int height) : fractal(new Fractal(width, height)) { }

FractalBuilder & FractalBuilder::setScale(long double minR, long double maxR, long double minI, long double maxI){
    return *this;
}

FractalBuilder & FractalBuilder::setAlgorithm(unique_ptr<FractalAlgorithm> alg){
    return *this;
}

template<typename T>
FractalBuilder & FractalBuilder::setGradient(T&& gradient){
    return *this;
}

FractalBuilder & FractalBuilder::setMaxIterations(int mxIter){
    return *this;
}

unique_ptr<Fractal> FractalBuilder::build(){
    return move(fractal);
}