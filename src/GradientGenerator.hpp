#ifndef GRADIENT_GENERATOR_HEADER
#define GRADIENT_GENERATOR_HEADER

#include <vector>
#include <utility>
#include "RGB.hpp"
using namespace std;

class GradientGenerator
{
    public:
        static vector<RGB> generateGradientMap(vector <pair <double, RGB> > gradient_points, int map_size);
    private:
        GradientGenerator() = default;
};

#endif // GRADIENT_GENERATOR_HEADER