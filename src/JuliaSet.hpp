#ifndef JULIA_SET_HEADER
#define JULIA_SET_HEADER

#include "Complex.hpp"
#include "CompareDoubles.hpp"
#include "FractalAlgorithm.hpp"
#include <utility>
using namespace std;

class JuliaSet : public FractalAlgorithm
{
    public:
        JuliaSet();
        JuliaSet(const Complex &inc);
        JuliaSet(const int &exponent);
        JuliaSet(const int &exponent, const Complex &inc);

        pair<int, tuple <Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) override;
        int getExponent() override;
    private:
        Complex m_inc;
        int m_exponent = 2;
        bool m_pixel_as_inc = true;
};

#endif // JULIA_SET HEADER