#ifndef FRACTAL_ALGORITHM_CREATOR
#define FRACTAL_ALGORITHM_CREATOR

#include "FractalAlgorithm.hpp"
#include "Mandelbrot.hpp"
#include "JuliaSet.hpp"
#include "Newton.hpp"

/**
 * @brief Set of factory functions for creating fractal algorithms
 * 
 */
class FractalAlgorithmCreator
{
    public:
        /**
         * @brief Disable possibility creating #FractalAlgorithmCreator objects
         * 
         */
        FractalAlgorithmCreator() = delete;

        /**
         * @brief #Mandelbrot factory function
         * 
         * @return pointer to the new instance of #Mandelbrot class
         */
        static unique_ptr<FractalAlgorithm> createMandelbrot(){
            return make_unique<Mandelbrot>();
        }
        
        /**
         * @brief Multibrot factory function
         * 
         * @param exponent - multibrot polynomial exponent
         * @return pointer to the new instance of #Mandelbrot class with Mulitbrot mode
         */
        static unique_ptr<FractalAlgorithm> createMultibrot(const int exponent){
            return make_unique<Mandelbrot>(exponent);
        }

        /**
         * @brief #JuliaSet base factory function
         * 
         * @return pointer to the new instance of #JuliaSet class
         */
        static unique_ptr<FractalAlgorithm> createJuliaSet(){
            return make_unique<JuliaSet>();
        }

        /**
         * @brief #JuliaSet factory function
         * 
         * @param incremenation - complex number used to increment by #JuliaSet algorithm
         * @return pointer to the new instance of #JuliaSet class
         */
        static unique_ptr<FractalAlgorithm> createJuliaSet(const Complex &incremenation){
            return make_unique<JuliaSet>(incremenation);
        }

        /**
         * @brief PolynomialJuliaSet factory function 
         * 
         * @param exponent - #JuliaSet polynomial exponent used to computing next values
         * @return pointer to the new instance of #JuliaSet class with PolynomialJuliaSet mode
         */
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(const int exponent){
            return make_unique<JuliaSet>(exponent);
        }

        /**
         * @brief PolynomialJuliaSet factory function
         * 
         * @param exponent - #JuliaSet polynomial exponent used to computing next values
         * @param incrementation - complex number used to increment by PolynomialJuliaSet algorithm
         * @return pointer to the new instance of #JuliaSet class with PolynomialJuliaSet mode 
         */
        static unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(const int exponent, const Complex &incrementation){
            return make_unique<JuliaSet>(exponent, incrementation);
        }

        /**
         * @brief #Newton factory function
         * 
         * @return pointer to the new instance of #Newton class 
         */
        static unique_ptr<FractalAlgorithm> createNewton(){
            return make_unique<Newton>();
        }

        /**
         * @brief #Newton factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - used to computing values by #Newton algorithm
         * @return pointer to the new instance of #Newton class 
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor"); 
            return make_unique<Newton>(forward<T>(polynomial));
        }

        /**
         * @brief #Newton factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - used to computing values by #Newton algorithm
         * @param relaxation - scalar value using to multiplying next values of iterations
         * @return pointer to the new instance of #Newton class 
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNewton(T&& polynomial, const Complex &relaxation){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor"); 
            return make_unique<Newton>(forward<T>(polynomial), relaxation);
        }

        /**
         * @brief Nova factory function
         * 
         * @return pointer to the new instance of #Newton class with Nova mode
         */
        static unique_ptr<FractalAlgorithm> createNova(){
            return make_unique<Newton>(true, false);
        }

        /**
         * @brief Nova factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - polynomial used to computing values by Nova algorithm
         * @return pointer to the new instance of #Newton class with Nova mode
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), true, false);
        }

        /**
         * @brief Nova factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - polynomial used to computing values by Nova algorithm
         * @param relaxation - scalar value using to multiplying next values of iterations
         * @return pointer to the new instance of #Newton class with Nova mode
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, true, false);
        }

        /**
         * @brief Nova factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - polynomial used to computing values by Nova algorithm
         * @param relaxation - scalar value using to multiplying next values of iterations
         * @param startval - starting value z0 denoting value of first iteration
         * @return pointer to the new instance of #Newton class with Nova mode
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation, const Complex &startval){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, startval);
        }

        /**
         * @brief Nova factory function
         * 
         * @tparam T - the type of polynomial, #Newton has to be constructible from object of this type
         * @param polynomial - polynomial used to computing values by Nova algorithm
         * @param relaxation - scalar value using to multiplying next values of iterations
         * @param pixstart - determine whether the value of the first iteration should be a complex number
         *                   given as a parameter of FractalAlgorithm::getIterationsAndOrbit(const Complex &) function 
         * @return pointer to the new instance of #Newton class with Nova mode
         */
        template <typename T>
        static unique_ptr<FractalAlgorithm> createNova(T&& polynomial, const Complex &relaxation, const bool pixstart){
            static_assert(is_constructible<Newton, T>::value, 
                          "Can't pass given polynomial to Newton class constructor");
            return make_unique<Newton>(forward<T>(polynomial), relaxation, true, pixstart);
        }
};

#endif // FRACTAL_ALGORITHM_CREATOR
