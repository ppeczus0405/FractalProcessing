#ifndef FRACTAL_ALGORITHM_CREATOR
#define FRACTAL_ALGORITHM_CREATOR

#include "FractalAlgorithm.hpp"
#include "Mandelbrot.hpp"
#include "JuliaSet.hpp"

class FractalAlgorithmCreator
{
    public:
        FractalAlgorithmCreator() = delete;

        // Fabric functions for creating specific FractalAlgorithms
        // Mandelbrot
        static unique_ptr<FractalAlgorithm> createMandelbrot(){
            return make_unique<Mandelbrot>();
        }
        
        // Multibrot
        template <typename T>
        static unique_ptr<FractalAlgorithm> createMultibrot(T&& exponent){
            return make_unique<Mandelbrot>(forward<T>(exponent));
        }

        // Julia Set
        static unique_ptr<FractalAlgorithm> createJuliaSet(){
            return make_unique<JuliaSet>();
        }

        static unique_ptr<FractalAlgorithm> createJuliaSet(const Complex &incremenation){
            return make_unique<JuliaSet>(incremenation);
        }

        static unique_ptr<FractalAlgorithm> createJuliaSet(Complex &&incremenation){
            return make_unique<JuliaSet>(move(incremenation));
        }

        // Polynomial Julia Set
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(const int &exponent){
            return make_unique<JuliaSet>(exponent);
        }

        template <typename T, typename U>
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(T&& exponent, U&& incrementation){
            return make_unique<JuliaSet>(forward<T>(exponent), forward<U>(incrementation));
        }
};

#endif // FRACTAL_ALGORITHM_CREATOR
