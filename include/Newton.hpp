#ifndef NEWTON_HEADER
#define NEWTON_HEADER

#include <functional>
#include "FractalAlgorithm.hpp"
using namespace std;

class Newton : public FractalAlgorithm
{
    public:
        Newton(bool nova = false, bool pixstart = true);
        Newton(const vector <Complex> &polynomial, bool nova = false, bool pixstart = true);
        Newton(const vector <Complex> &polynomial, const Complex &relaxation, bool nova = false, bool pixstart = true);
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
        bool is_nova = false;
        bool pixel_start = false;
};

#endif // NEWTON_HEADER