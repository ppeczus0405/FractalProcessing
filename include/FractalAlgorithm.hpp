#ifndef FRACTAL_ALGORITHM
#define FRACTAL_ALGORITHM
 
#include "Complex.hpp"
#include <memory>
#include <utility>
using namespace std;
 
constexpr int MAX_ITERATIONS = 1e4;
constexpr int MIN_ITERATIONS = 1;
constexpr int DEFAULT_ITERATIONS = 35;

constexpr long double DIVERGENCE_BAILOUT = 1e8;
constexpr long double CONVERGENCE_BAILOUT = 1e-14;
 
enum class FractalAlgorithmType{
    MANDELBROT,
    MULTIBROT,
    JULIA,
    POLYJULIA,
    NEWTON,
    NOVA
};
 
class FractalAlgorithm
{
    public:
        FractalAlgorithm(FractalAlgorithmType falg) : algorithmType(falg) { }
        
        virtual pair<int, tuple<Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) = 0;
        virtual int getExponent() = 0;
 
        inline bool setMaxIterationsNumber(int n){
            // We can only change value if it makes sense
            if(n >= MIN_ITERATIONS && n <= MAX_ITERATIONS){
                max_iter = n;
                return true;
            }
            else{
                cerr << "Iteration value have to be integer meeting the condition: ";
                cerr << MIN_ITERATIONS << " <= iterations <= " << MAX_ITERATIONS << endl;
                cerr << "Not changed. Value = " << max_iter << endl;
                return false;
            }
        }
 
        inline FractalAlgorithmType getAlgorithmType(){
            return algorithmType;
        }
 
        inline int getMaxIterationsNumber() const{
            return max_iter;
        }
 
    protected:
        int max_iter = DEFAULT_ITERATIONS;
    private:
        FractalAlgorithmType algorithmType;
};
 
#endif // FRACTAL_ALGORITHM