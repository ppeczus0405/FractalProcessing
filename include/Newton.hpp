#ifndef NEWTON_HEADER
#define NEWTON_HEADER

#include <functional>
#include "FractalAlgorithm.hpp"
using namespace std;

/**
 * @brief Implementation of Newton and Nova algorithm
 * 
 */
class Newton : public FractalAlgorithm
{
    public:
        /**
         * @brief Construct a new Newton object
         * @see FractalAlgorithmCreator::createNewton()\n 
         *      FractalAlgorithmCreator::createNova()
         */
        Newton(bool nova = false, bool pixstart = true);\

        /**
         * @brief Construct a new Newton object
         * @see FractalAlgorithmCreator::createNewton(T&&)\n
         *      FractalAlgorithmCreator::createNova(T&&)
         */
        Newton(const vector <Complex> &polynomial, bool nova = false, bool pixstart = true);

        /**
         * @brief Construct a new Newton object
         * @see FractalAlgorithmCreator::createNewton(T&&, const Complex &)\n
         *      FractalAlgorithmCreator::createNova(T&&, const Complex &)\n
         *      FractalAlgorithmCreator::createNova(T&&, const Complex &, const bool)
         */
        Newton(const vector <Complex> &polynomial, const Complex &relaxation, bool nova = false, bool pixstart = true);

        /**
         * @brief Construct a new Newton object
         * @see FractalAlgorithmCreator::createNova(T&&, const Complex &, const Complex &)
         */
        Newton(const vector <Complex> &polynomial, const Complex &relaxation, const Complex &incrementation);

        pair <int, tuple <Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) override;
        int getExponent() override;
        
    private:
        function<Complex(Complex)> f = nullptr;
        function<Complex(Complex)> fdx = nullptr;
        
        function<Complex(Complex)> createPolynomialFunction(const vector <Complex> &polynomial);
        void initialize_functions();
        void computeDerivative();

        vector <Complex> m_polynomial, m_derivative;
        Complex relax = Complex::ONE;
        Complex start_value = Complex::ONE;
        bool is_nova = false; // is_nova = false -> Newton fractal, otherwise Nova fractal
        bool pixel_start = false; // is start value z0 of iteration equal pixel value
};

#endif // NEWTON_HEADER