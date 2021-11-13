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
     * @brief Handles monotonic interpolation of colors
     * 
     * @details Class enables monotonic interpolation of points.\n 
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
             * @param x - coordinate on the OX axis
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
             * @brief Gradient defined as WHITE -> BLACK interpolation
             * 
             */
            static const Gradient WHITE_BLACK;


            /**
             * @brief Insert new point.
             * 
             * @param x - coordinate on the OX axis
             * @param color - RGB value
             * @return true if there are no points at the x position. In this case new point will be inserted.
             * @return false if there is already point at the position x. In this case nothing happen.
             */
            bool insertPoint(const double x, const RGB &color);

            /**
             * @brief Change color value of point
             * 
             * @param x - coordinate on the OX axis
             * @param color - new RGB value
             * @return true if there is the point at the position x. In this case value of this point will be changed.
             * @return false if there are no point at given x position. In this case nothing happen
             */
            bool changePoint(const double x, const RGB &color);
            
            /**
             * @brief Check if point isInside
             * 
             * @param x - coordinate on the OX axis
             * @return true in case when point is inside
             * @return false otheriwse
             */
            bool isPointInside(const double x) noexcept;
            
            /**
             * @brief Remove point if exists
             * 
             * @param x - coordinate on the OX axis
             * @return true in case when point is inside. Point is successfully removed.
             * @return false in case when there is no point inside. Nothing happen
             */
            bool removePoint(const double x);
            
            /**
             * @brief Get the color value of point
             * 
             * @param x - coordinate on the OX axis
             * @throw invalid_argument in case when there is no point with given x coordinate 
             * 
             * @return value of the color
             * 
             */
            RGB getPointColor(const double x);
            
            /**
             * @brief Get all points inside gradient
             * 
             * @return vector with wanted points
             */
            vector <pair <double, RGB> > getPoints() noexcept;
            
            /**
             * @brief Generating gradient map with given size.
             *        
             * @details Gradient map is vector of colors defined as follows:\n
             *        For map_size = 0 -> empty vector\n
             *        Otherwise let M is resulting vector and i is any index from range [0 ... map_size - 1]\n
             *        Then depending on the number of points inside gradient, the following occurs:
             *        - 0 points inside -> M[i] = BLACK
             *        - 1 points inside -> M[i] = COLOR_OF_POINT
             *        - 2 points inside and more -> M[i] = value of interpolation function for i-th x coordinate inside range [MIN_X, MAX_X]   
             * 
             * @param map_size - size of gradient map
             * @return vector with interpolated colors
             */
            vector<RGB> generateGradientMap(int map_size);

            /**
             * @brief Save gradient to file
             * 
             * @param filename - path where file is going to save
             * @param w - width of image
             * @param h - height of image
             * @return true when successfully saved the file
             * @return false otherwise
             */
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