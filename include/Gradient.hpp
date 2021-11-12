#ifndef GRADIENT_HEADER
#define GRADIENT_HEADER

#include "RGB.hpp"
#include "spline.hpp"
#include "Image.hpp"
#include "ImageSaver.hpp"
using namespace std;

namespace PekiProcessing
{
    /**
     * @brief Target of the interpolation(each color is interpolated separately). 
     * 
     */
    enum class InterpolationTarget : uint8_t{
        RED,
        GREEN,
        BLUE
    };

    /**
     * @brief Handling monotonic interpolation of colors
     * 
     * @details Class enables monotonic interpolation pairs of points.\n 
     *          Points are defined as (x, y) where:\n
     *          - x is coordinate on the OX axis\n 
     *          - f(x) - value of the color given in RGB representation\n
     *          
     */
    class Gradient
    {
        public:
            /**
             * @brief Construct gradient object with no point inside. Just a black surface
             * 
             */
            Gradient() = default;
            
            /**
             * @brief Construct gradient object with one point. Gradient surface has a color defined by this point.
             * 
             * @param x - position of color
             * @param color - RGB value
             */
            Gradient(const double x, const RGB &color);

            /**
             * @brief Construct gradient object defined by given points.
             * 
             * @param gradient_points - points to interpolate
             */
            Gradient(const vector < pair <double, RGB> > &gradient_points);
            
            /**
             * @brief Gradient defined as white -> black interpolation
             * 
             */
            static const Gradient WHITE_BLACK;


            /**
             * @brief Insert new point.
             * 
             * @param x - position of color
             * @param color - RGB value
             * @return true if there are no points at the x position. In this case new point will be inserted
             * @return false if there is already point at the position x. In this case nothing happen.
             */
            bool insertPoint(const double x, const RGB &color);

            /**
             * @brief Change color value of point
             * 
             * @param x - position of color
             * @param color - new RGB value
             * @return true if there is the point at the position x. In this case value of this point will be changed.
             * @return false if there are no point at given x position. In this case nothing happen
             */
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