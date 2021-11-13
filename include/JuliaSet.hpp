#ifndef JULIA_SET_HEADER
#define JULIA_SET_HEADER

#include "Complex.hpp"
#include "CompareDoubles.hpp"
#include "FractalAlgorithm.hpp"
#include <utility>
using namespace std;

/**
 * @brief Implementation of JuliaSet and PolynomialJuliaSet algorithm
 * 
 */
class JuliaSet : public FractalAlgorithm
{
    public:
        /**
         * @brief Construct a new Julia Set object
         * @see FractalAlgorithmCreator::createJuliaSet()
         */
        JuliaSet();

        /**
         * @brief Construct a new Julia Set object
         * @see FractalAlgorithmCreator::createJuliaSet(const Complex &)
         */
        JuliaSet(const Complex &inc);

        /**
         * @brief Construct a new Julia Set object
         * @see FractalAlgorithmCreator::createPolynomialJuliaSet(const int)
         */
        JuliaSet(const int exponent);

        /**
         * @brief Construct a new Julia Set object
         * @see FractalAlgorithmCreator::createPolynomialJuliaSet(const int, const Complex &)
         */
        JuliaSet(const int exponent, const Complex &inc);

        pair<int, tuple <Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) override;
        int getExponent() override;
    private:
        Complex m_inc;
        int m_exponent = 2;
        bool m_pixel_as_inc = true;
};

#endif // JULIA_SET HEADER