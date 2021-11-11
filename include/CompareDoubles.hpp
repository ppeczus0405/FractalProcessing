#ifndef COMPARE_DOUBLES
#define COMPARE_DOUBLES

#include <cmath>

/**
 * @brief Comparing floating points numbers with defined precision 
 * 
 * @details Designed as set of static functions without instantiating possibility.
 */
class CompareDoubles
{
    public:
        /**
         * @brief Disable possibility creating CompareDoubles objects
         * 
         */
        CompareDoubles() = delete;
        
        /**
         * @brief Precision included in comparing long doubles numbers
         * 
         */
        constexpr static long double EPSILON_LONG = 1e-10;

        /**
         * @brief Precision included in comparing double numbers
         * 
         */
        constexpr static double EPSILON = 1e-7;

        /**
         * @brief Check equality of two double numbers using including precision
         * 
         * @param a - first number
         * @param b - second number 
         * @return true in case when a is equal b
         * @return false in case when a is not equal b 
         * @see isEqual(const long double, const long double)
         */
        static bool isEqual(const double a, const double b){
            return fabs(a - b) < EPSILON;
        }
        
        /**
         * @brief Overload of isEqual(const double, const double) for long double numbers
         * 
         * @param a - first number
         * @param b - second number 
         * @return true in case when a is equal b
         * @return false in case when a is not equal b 
         * @see isEqual(const double, const double)
         */
        static bool isEqual(const long double a, const long double b){
            return fabs(a - b) < EPSILON_LONG;
        }

        /**
         * @brief Check if a is greater than b including precision
         * 
         * @param a - first number
         * @param b - second number
         * @return true in case when a is greater than b
         * @return false otherwise
         * @see isGreater(const long double, const long double) 
         */
        static bool isGreater(const double a, const double b){
            return a - b >= EPSILON;
        }

        /**
         * @brief Overload of isGreater(const double, const double) for long double numbers
         * 
         * @param a - first number
         * @param b - second number
         * @return true in case when a is greater than b
         * @return false otherwise
         * @see isGreater(const double, const double)
         */
        static bool isGreater(const long double a, const long double b){
            return a - b >= EPSILON_LONG;
        }

        /**
         * @brief Check if a is lesser than b including precision
         * 
         * @param a - first number
         * @param b - second number
         * @return true in case when a is lesser than b
         * @return false otherwise
         * @see isLesser(const long double, const long double)
         */
        static bool isLesser(const double a, const double b){
            return a - b <= -EPSILON;
        }

        /**
         * @brief Overload of isLesser(const double, const double) for long double numbers
         * 
         * @param a - first number
         * @param b - second number
         * @return true in case when a is lesser than b
         * @return false otherwise
         * @see isLesser(const double, const double)
         */
        static bool isLesser(const long double a, const long double b){
            return a - b <= -EPSILON_LONG;
        }
};

#endif // COMPARE_DOUBLES