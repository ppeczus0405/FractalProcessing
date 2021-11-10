#ifndef FRACTAL_ALGORITHM_CREATOR
#define FRACTAL_ALGORITHM_CREATOR

#include "FractalAlgorithm.hpp"
#include "Mandelbrot.hpp"
#include "JuliaSet.hpp"
#include "Newton.hpp"

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

        static unique_ptr<FractalAlgorithm> createJuliaSet(Complex&& incremenation){
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

        // Newton Fractal
        static unique_ptr<FractalAlgorithm> createNewton(){
            return make_unique<Newton>();
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial){
            return make_unique<Newton>(forward<T>(polynomial));
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial, const Complex &relaxation){
            return make_unique<Newton>(forward<T>(polynomial), relaxation);
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial, Complex&& relaxation){
            return make_unique<Newton>(forward<T>(polynomial), move(relaxation));
        }

        // Nova Fractal
        static unique_ptr<FractalAlgorithm> createNova(){
            return make_unique<Newton>(true, false);
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial){
            return make_unique<Newton>(forward<T>(polynomial), true, false);
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation){
            return make_unique<Newton>(forward<T>(polynomial), relaxation, true, false);
        }

        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, Complex&& relaxation){
            return make_unique<Newton>(forward<T>(polynomial), move(relaxation), true, false);
        }

        template <typename T, typename U>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, U&& relaxation, const Complex &startval){
            return make_unique<Newton>(forward<T>(polynomial), forward<U>(relaxation), startval);
        }

        template <typename T, typename U>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, U&& relaxation, Complex&& startval){
            return make_unique<Newton>(forward<T>(polynomial), forward<U>(relaxation), move(startval));
        }

        template <typename T, typename U>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, U&& relaxation, const bool pixstart){
            return make_unique<Newton>(forward<T>(polynomial), forward<U>(relaxation), true, pixstart);
        }
};

#endif // FRACTAL_ALGORITHM_CREATOR
