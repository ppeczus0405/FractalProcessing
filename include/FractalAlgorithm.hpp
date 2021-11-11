#ifndef FRACTAL_ALGORITHM
#define FRACTAL_ALGORITHM

#include "Complex.hpp"
#include <memory>
#include <utility>
using namespace std;

/**
 * @brief Maximum possible number of iterations - limited due the performance
 * 
 */
constexpr int MAX_ITERATIONS = 1e4;

/**
 * @brief Minimum possible number of iterations
 * 
 */
constexpr int MIN_ITERATIONS = 1;

/**
 * @brief Default number of iterations
 * 
 */
constexpr int DEFAULT_ITERATIONS = 35;

/**
 * @brief Divergence coloring bailout value
 * 
 */
constexpr long double DIVERGENCE_BAILOUT = 1e8;

/**
 * @brief Convergence coloring bailout value
 * 
 */
constexpr long double CONVERGENCE_BAILOUT = 1e-14;

/**
 * @brief Kinds of available fractal algorithms
 * 
 */
enum class FractalAlgorithmType{
    MANDELBROT,
    MULTIBROT,
    JULIA,
    POLYJULIA,
    NEWTON,
    NOVA
};

/**
 * @brief Abstract class used as a base of different algorithms classes
 * 
 */
class FractalAlgorithm
{
    public:
        /**
         * @brief Set algorithm type
         * 
         * @param falg - kind of the algorithm
         */
        FractalAlgorithm(FractalAlgorithmType falg) : algorithmType(falg) { }
        
        /**
         * @brief Get number of iterations and last 3 elements of orbit trap
         * 
         * @param c - complex number
         * @return pair consist of number of iterations and 3 last elements of orbit trap where:\n
         *              - last element is first iteration value exceeding bailout value,\n
         *              - second element is iteration value before last element,\n
         *              - first element is iteration value before second element,\n
         *         Number of iterations denote how many iterations needs algorithm perform to fall into the orbit trap.
         *         If the algorithm don't fall into the orbit trap and maximum number of iterations was reached then maximal
         *         number of iterations should be returned
         */
        virtual pair<int, tuple<Complex, Complex, Complex> > getIterationsAndOrbit(const Complex &c) = 0;
        
        /**
         * @brief Get exponent of polynomial used to compute next iteration value
         * 
         * @return exponent of polynomial
         */
        virtual int getExponent() = 0;

        /**
         * @brief Set maximum number of iterations that the algorithm can perform.
         * 
         * @param n - maximum number of iterations
         * @return true when setted value successfully
         * @return false when n < #MIN_ITERATIONS or n > #MAX_ITERATIONS
         */
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

        /**
         * @brief Get the kind of the algoritm
         * 
         * @return the algorithm type
         */
        inline FractalAlgorithmType getAlgorithmType(){
            return algorithmType;
        }

        /**
         * @brief Get maximum number of iterations
         * 
         * @return maximum number of iterations
         */
        inline int getMaxIterationsNumber() const{
            return max_iter;
        }

    protected:
        /**
         * @brief maximum number of iterations. It's upper bound of iterations that algorithm can perform.
         * 
         */
        int max_iter = DEFAULT_ITERATIONS;
    private:
        /**
         * @brief Kind of algorithm
         * 
         */
        FractalAlgorithmType algorithmType;
};

#endif // FRACTAL_ALGORITHM