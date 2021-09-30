#include "Newton.hpp"
#include "CompareDoubles.hpp"
#include <ranges>

const Complex Newton::NEUTRAL_INCREMENT = Complex::ZERO;
const Complex Newton::NEUTRAL_RELAXATION = Complex::ONE;

// Newton constructors
Newton::Newton() : pixel_as_inc(false)
{
    // z^3 - 1 = 0
    m_polynomial = {Complex::ONE, Complex::ZERO, Complex::ZERO, -Complex::ONE};
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial) : m_polynomial(polynomial), pixel_as_inc(false)
{
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relaxation) : 
               m_polynomial(polynomial), relax(relaxation), pixel_as_inc(false)
{
    initialize_functions();
}

// Nova constructors
Newton::Newton(bool pixelInc) : pixel_as_inc(pixelInc)
{
    // z^3 - 1 = 0
    m_polynomial = {Complex::ONE, Complex::ZERO, Complex::ZERO, -Complex::ONE};
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, bool pixelInc) : m_polynomial(polynomial), pixel_as_inc(pixelInc)
{
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relaxation, bool pixelInc) :
               m_polynomial(polynomial), relax(relaxation), pixel_as_inc(pixelInc) 
{
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relaxation, const Complex &incrementation) :
               m_polynomial(polynomial), relax(relaxation), inc(incrementation), pixel_as_inc(false)
{
    initialize_functions();
}

void Newton::initialize_functions(){
    computeDerivative();
    f = createPolynomialFunction(m_polynomial);
    fdx = createPolynomialFunction(m_derivative);
}

void Newton::computeDerivative(){
    m_derivative = m_polynomial;
    if((int)m_polynomial.size() == 0) // empty polynomial is treated as W(c) = 0, where c is complex number
        return;
    m_derivative.pop_back();
    int exponent = 1;
    for(auto &element : m_derivative | views::reverse)
        element *= exponent++;
}

function<Complex(Complex)> Newton::createPolynomialFunction(const vector <Complex> &polynomial){
    function <Complex(Complex)> func = [&polynomial](Complex c){
        Complex ans = Complex::ZERO;
        Complex z = Complex::ONE;
        for(const auto &element : polynomial | views::reverse){
            ans += z * element;
            z *= c;
        }
        return ans;
    };
    return func;
}

pair <int, tuple <Complex, Complex, Complex> > Newton::getIterations(const Complex &c){
    auto nextIter = [&](Complex z){
        Complex dx_value = fdx(z);
        if(dx_value == Complex::ZERO)
            return make_pair(false, Complex::ZERO);
        return make_pair(true, z - relax * (f(z) / dx_value) + (pixel_as_inc ? c : inc));
    };
    auto checkEndPoint = [](Complex a, Complex b){
        return Complex::absolute_square(b - a) / max(1.0L, Complex::absolute_square(b)) <= CONVERGENCE_BAILOUT;
    };

    int iters = 0;
    bool notEnd = true;
    tuple <Complex, Complex, Complex> three_orbit(c, c, c);
    
    while(notEnd && iters < max_iter){
        auto next = nextIter(get<2>(three_orbit));
        // Encounter case when derivative is equal to zero 
        if(!next.first) return make_pair(-1, three_orbit);
        
        get<0>(three_orbit) = get<1>(three_orbit);
        get<1>(three_orbit) = get<2>(three_orbit);
        get<2>(three_orbit) = next.second;
        notEnd = !checkEndPoint(get<1>(three_orbit), get<2>(three_orbit));
        iters++;
    }
    return {iters, three_orbit};
}