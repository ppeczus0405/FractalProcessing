#include <iostream>
#include "Fractal.hpp"
#include "config.hpp"
using namespace std;

void process_gradient(PekiProcessing::Gradient &g, const int i){
    // ${PROJECT_PATH}/generated_images/gradient${i}/
    const string path = Config::SAVE_PATH + Config::OS_SEPARATOR + "gradient" + to_string(i) + Config::OS_SEPARATOR;
    filesystem::create_directories(path);
    g.write(path + "gradient");

    const int resolution = 512;
    unique_ptr<Fractal> frac = FractalBuilder(resolution, resolution).setGradient(g).setGradientMapSize(1024).build();
    
    // Mandelbrot
    frac->setAlgorithm(FractalAlgorithmCreator::createMandelbrot());
    frac->write(path + "mandelbrot", SaveFormat::JPEG);

    // Multibrot
    frac->setAlgorithm(FractalAlgorithmCreator::createMultibrot(4));
    frac->write(path + "multibrot", SaveFormat::JPEG);

    // Julia Set
    frac->setAlgorithm(FractalAlgorithmCreator::createJuliaSet(Complex(-0.8L, 0.156L)));
    frac->setIterations(100);
    frac->write(path + "juliaset", SaveFormat::JPEG);

    // Poly Julia Set
    frac->setAlgorithm(FractalAlgorithmCreator::createPolynomialJuliaSet(6, Complex(0.0L, -0.8L)));
    frac->write(path + "polyjuliaset", SaveFormat::JPEG);

    // Newton
    // z^8 - 3z^4 - 4
    vector<Complex> polynomial{Complex::ONE, Complex::ZERO, Complex::ZERO, Complex::ZERO, Complex(3.0L, 0.0L),
                               Complex::ZERO, Complex::ZERO, Complex::ZERO, Complex(-4.0L, 0.0L)};
    frac->setAlgorithm(FractalAlgorithmCreator::createNewton(polynomial));
    frac->write(path + "newton", SaveFormat::JPEG);

    // Nova
    frac->setAlgorithm(FractalAlgorithmCreator::createNova());
    frac->setScale(-1.8L, 0.65L, -0.9L, 0.9L);
    frac->setIterations(150);
    frac->write(path + "nova", SaveFormat::JPEG);
}

int main(int argc, char** argv)
{
    filesystem::create_directory(Config::SAVE_PATH);
    PekiProcessing::Gradient gradient1, gradient2, gradient3;
    
    // GRADIENT 1
    gradient1.insertPoint(0.0, {0, 7, 100});
    gradient1.insertPoint(0.16, {32, 107, 203});
    gradient1.insertPoint(0.42, {237, 255, 255});
    gradient1.insertPoint(0.6425, {255, 170, 0});
    gradient1.insertPoint(0.8575, {0, 2, 0});
    
    // GRADIENT 2
    gradient2.insertPoint(0.0, {92, 13, 13});
    gradient2.insertPoint(0.16, {236, 72, 72});
    gradient2.insertPoint(0.42, {237, 255, 255});
    gradient2.insertPoint(0.6425, {59, 68, 255});
    gradient2.insertPoint(0.8575, {0, 2, 0});

    ///* GRADIENT 3
    gradient3.insertPoint(0.0, {122, 0, 148});
    gradient3.insertPoint(0.16, {225, 111, 156});
    gradient3.insertPoint(0.42, {237, 255, 255});
    gradient3.insertPoint(0.6425, {152, 233, 0});
    gradient3.insertPoint(0.8575, {0, 2, 0});

    process_gradient(gradient1, 1);
    process_gradient(gradient2, 2);
    process_gradient(gradient3, 3);
    return 0;
}