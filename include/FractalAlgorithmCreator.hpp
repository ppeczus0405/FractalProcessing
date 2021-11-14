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
 
        static unique_ptr<FractalAlgorithm> createMandelbrot(){
            return make_unique<Mandelbrot>();
        }
        
        static unique_ptr<FractalAlgorithm> createMultibrot(const int exponent){
            return make_unique<Mandelbrot>(exponent);
        }
 
        static unique_ptr<FractalAlgorithm> createJuliaSet(){
            return make_unique<JuliaSet>();
        }
 
        static unique_ptr<FractalAlgorithm> createJuliaSet(const Complex &incremenation){
            return make_unique<JuliaSet>(incremenation);
        }
 
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(const int exponent){
            return make_unique<JuliaSet>(exponent);
        }
 
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(const int exponent, const Complex &incrementation){
            return make_unique<JuliaSet>(exponent, incrementation);
        }
 
        static unique_ptr<FractalAlgorithm> createNewton(){
            return make_unique<Newton>();
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor"); 
            return make_unique<Newton>(forward<T>(polynomial));
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial, const Complex &relaxation){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor"); 
            return make_unique<Newton>(forward<T>(polynomial), relaxation);
        }
 
        static unique_ptr<FractalAlgorithm> createNova(){
            return make_unique<Newton>(true, false);
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), true, false);
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, true, false);
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation, const Complex &startval){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, startval);
        }
 
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation, const bool pixstart){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, true, pixstart);
        }
};
 
#endif // FRACTAL_ALGORITHM_CREATOR
