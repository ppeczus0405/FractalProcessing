#ifndef NEWTON_HEADER
#define NEWTON_HEADER

#include <functional>
#include "FractalAlgorithm.hpp"
using namespace std;

class Newton : public FractalAlgorithm
{
    public:
        Newton();
        Newton(const vector <Complex> &polynomial);
        Newton(const vector <Complex> &polynomial, const Complex &relax);
        Newton(const vector <Complex> &polynomial, const Complex &relax, const Complex &incrementation);

        static const Complex NEUTRAL_INCREMENT;
        static const Complex NEUTRAL_RELAXATION;

        pair <int, Complex> getIterations(const Complex &c) override;

    private:
        function<Complex(Complex)> f = nullptr;
        function<Complex(Complex)> fdx = nullptr;
        
        Complex relax = NEUTRAL_RELAXATION;
        Complex inc = NEUTRAL_INCREMENT;
};

#endif // NEWTON_HEADER