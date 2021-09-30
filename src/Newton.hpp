#ifndef NEWTON_HEADER
#define NEWTON_HEADER

#include <functional>
#include "FractalAlgorithm.hpp"
using namespace std;

class Newton : public FractalAlgorithm
{
    public:
        // Newton fractal initialization constructors
        Newton();
        Newton(const vector <Complex> &polynomial);
        Newton(const vector <Complex> &polynomial, const Complex &relaxation);
        // Nova fractal initialization constructors
        Newton(bool pixelInc);
        Newton(const vector <Complex> &polynomial, bool pixelInc);
        Newton(const vector <Complex> &polynomial, const Complex &relaxation, bool pixelInc);
        Newton(const vector <Complex> &polynomial, const Complex &relaxation, const Complex &incrementation);

        static const Complex NEUTRAL_INCREMENT;
        static const Complex NEUTRAL_RELAXATION;

        pair <int, tuple <Complex, Complex, Complex> > getIterations(const Complex &c) override;

    private:
        function<Complex(Complex)> f = nullptr;
        function<Complex(Complex)> fdx = nullptr;
        
        function<Complex(Complex)> createPolynomialFunction(const vector <Complex> &polynomial);
        void initialize_functions();
        void computeDerivative();

        vector <Complex> m_polynomial, m_derivative;
        Complex relax = NEUTRAL_RELAXATION;
        Complex inc = NEUTRAL_INCREMENT;
        bool pixel_as_inc = true;
};

#endif // NEWTON_HEADER