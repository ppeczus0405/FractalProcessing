#ifndef GRADIENT_HEADER
#define GRADIENT_HEADER

#include "RGB.hpp"
#include "spline.hpp"
#include "Image.hpp"
#include "ImageSaver.hpp"
using namespace std;

namespace PekiProcessing
{
    enum class InterpolationTarget : uint8_t{
        RED,
        GREEN,
        BLUE
    };

    // Gradient with no point inside is just black surface
    class Gradient
    {
        public:
            Gradient() = default;
            Gradient(const double x, const RGB &color);
            Gradient(const vector < pair <double, RGB> > &gradient_points);
            
            static const Gradient WHITE_BLACK;

            bool insertPoint(const double x, const RGB &color);
            bool changePoint(const double x, const RGB &color);
            bool isPointInside(const double x) noexcept;
            bool removePoint(const double x);
            
            RGB getPointColor(const double x);
            vector <pair <double, RGB> > getPoints() noexcept;
            vector<RGB> generateGradientMap(int map_size);
            bool write(string filename, int w = DEFAULT_SAVE_WIDTH, int h = DEFAULT_SAVE_HEIGHT);
        private:
            vector < pair <double, RGB> > grad_points;
            vector <tuple<tk::spline, int, int> > mono_intervals[RGB_SIZE];
            bool interpolated = false;

            vector <pair <double, RGB> >::iterator getPointIter(const double x) noexcept;
            void generateMonotoneIntervals();
            RGB interpolate(const double x);
    };
}

#endif // GRADIENT_HEADER