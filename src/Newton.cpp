#include "Newton.hpp"
#include "CompareDoubles.hpp"
#include <ranges>

// Newton constructors
Newton::Newton(bool nova, bool pixstart) : is_nova(nova), pixel_start(pixstart)
{
    // z^3 - 1 = 0
    m_polynomial = {Complex::ONE, Complex::ZERO, Complex::ZERO, -Complex::ONE};
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, bool nova, bool pixstart) : m_polynomial(polynomial), is_nova(nova), pixel_start(pixstart)
{
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relaxation, bool nova, bool pixstart) : 
               m_polynomial(polynomial), relax(relaxation), is_nova(nova), pixel_start(pixstart)
{
    initialize_functions();
}

Newton::Newton(const vector <Complex> &polynomial, const Complex &relaxation, const Complex &startval) :
               m_polynomial(polynomial), relax(relaxation), start_value(startval), is_nova(true), pixel_start(false)
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

pair <int, tuple <Complex, Complex, Complex> > Newton::getIterationsAndOrbit(const Complex &c){
    auto nextIter = [&](Complex z){
        Complex dx_value = fdx(z);
        if(dx_value == Complex::ZERO)
            return make_pair(false, Complex::ZERO);
        return make_pair(true, z - relax * (f(z) / dx_value) + (is_nova ? c : Complex::ZERO));
    };
    auto checkEndPoint = [](Complex a, Complex b){
        return Complex::absolute_square(b - a) / max(1.0L, Complex::absolute_square(b)) <= CONVERGENCE_BAILOUT;
    };

    int iters = 0;
    bool notEnd = true;
    Complex z0 = pixel_start ? c : start_value; 
    tuple <Complex, Complex, Complex> three_orbit(z0, z0, z0);
    

    while(notEnd && iters < max_iter){
        auto next = nextIter(get<2>(three_orbit));
        // Encounter case when derivative is equal to zero 
        if(!next.first) 
            return make_pair(-1, three_orbit);
        get<0>(three_orbit) = get<1>(three_orbit);
        get<1>(three_orbit) = get<2>(three_orbit);
        get<2>(three_orbit) = next.second;
        notEnd = !checkEndPoint(get<1>(three_orbit), get<2>(three_orbit));
        iters++;
    }
    return {iters, three_orbit};
}