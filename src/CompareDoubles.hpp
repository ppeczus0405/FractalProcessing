#ifndef COMPARE_DOUBLES
#define COMPARE_DOUBLES

#include <cmath>

constexpr double EPSILON = 1e-7;

class CompareDoubles
{
    public:
        static bool isEqual(const double a, const double b){
            return fabs(a - b) < EPSILON;
        }
        static bool isGreater(const double a, const double b){
            return a - b >= EPSILON;
        }
        static bool isLesser(const double a, const double b){
            return a - b <= -EPSILON;
        }
    private:
        CompareDoubles();
};

#endif // COMPARE_DOUBLES