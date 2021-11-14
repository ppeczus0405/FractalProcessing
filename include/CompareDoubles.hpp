#ifndef COMPARE_DOUBLES
#define COMPARE_DOUBLES
 
#include <cmath>
 
class CompareDoubles
{
    public:
        CompareDoubles() = delete;
        
        constexpr static long double EPSILON_LONG = 1e-10;
        constexpr static double EPSILON = 1e-7;
 
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