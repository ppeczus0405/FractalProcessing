#include <iostream>
#include <filesystem>
#include "Fractal.hpp"
using namespace std;

const string SAVE_PATH = "../generated_images";

int main(int argc, char** argv)
{
    filesystem::create_directory(SAVE_PATH);
    PekiProcessing::Gradient gradient;
    gradient.insertPoint(0.0, {0, 7, 100});
    gradient.insertPoint(0.16, {32, 107, 203});
    gradient.insertPoint(0.42, {237, 255, 255});
    gradient.insertPoint(0.6425, {255, 170, 0});
    gradient.insertPoint(0.8575, {0, 2, 0});
    gradient.write(SAVE_PATH + "/gradient");


    unique_ptr<Fractal> frac = FractalBuilder(512, 512).setGradient(gradient).build();
    
    // Mandelbrot
    frac->setAlgorithm(FractalAlgorithmCreator::createMandelbrot());
    frac->write(SAVE_PATH + "/mandelbrot", SaveFormat::JPEG);

    // Multibrot
    frac->setAlgorithm(FractalAlgorithmCreator::createMultibrot(4));
    frac->write(SAVE_PATH + "/multibrot", SaveFormat::JPEG);

    // Julia Set
    frac->setAlgorithm(FractalAlgorithmCreator::createJuliaSet(Complex(-0.8L, 0.156L)));
    frac->setIterations(75);
    frac->write(SAVE_PATH + "/juliaset", SaveFormat::JPEG);

    // Poly Julia Set
    frac->setAlgorithm(FractalAlgorithmCreator::createPolynomialJuliaSet(3));
    //frac->setIterations(75);
    frac->write(SAVE_PATH + "/polyjuliaset", SaveFormat::JPEG);

    // Newton
    frac->setAlgorithm(FractalAlgorithmCreator::createNewton());
    frac->write(SAVE_PATH + "/newton", SaveFormat::JPEG);

    // Nova
    frac->setAlgorithm(FractalAlgorithmCreator::createNova());
    frac->setIterations(150);
    frac->write(SAVE_PATH + "/nova", SaveFormat::JPEG);
    return 0;
}