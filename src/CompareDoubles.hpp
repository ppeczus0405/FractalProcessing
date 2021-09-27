#ifndef COMPARE_DOUBLES
#define COMPARE_DOUBLES

#include <cmath>

constexpr double EPSILON = 1e-7;
constexpr long double EPSILON_LONG = 1e-10;

class CompareDoubles
{
    public:
        CompareDoubles() = delete;
        static bool isEqual(const double a, const double b){
            return fabs(a - b) < EPSILON;
        }
        static bool isEqual(const long double a, const long double b){
            return fabs(a - b) < EPSILON_LONG;
        }
        static bool isGreater(const double a, const double b){
            return a - b >= EPSILON;
        }
        static bool isGreater(const long double a, const long double b){
            return a - b >= EPSILON_LONG;
        }
        static bool isLesser(const double a, const double b){
            return a - b <= -EPSILON;
        }
        static bool isLesser(const long double a, const long double b){
            return a - b <= -EPSILON_LONG;
        }
};

#endif // COMPARE_DOUBLES