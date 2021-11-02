#ifndef FRACTAL_COLORING
#define FRACTAL_COLORING

#include "Gradient.hpp"
#include "RGB.hpp"
#include "Complex.hpp"
using namespace std;
using namespace PekiProcessing;

constexpr int DEFAULT_COLOR_MAP_SIZE = 512;
constexpr int MAX_MAP_SIZE = 16384;

class FractalColoring{
    private:
        FractalColoring(int exponent) : 
            m_exponent(exponent) {}
        FractalColoring(int exponent, long double bailout) : FractalColoring(exponent) {
            m_bailout = bailout;
        }

    public:
        FractalColoring(int maxIterations, int exponent, long double bailout) : 
            FractalColoring(exponent, bailout)
        {
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        FractalColoring(int maxIterations, int exponent, long double bailout, int mapSize) : 
            FractalColoring(exponent, bailout)
        { 
            assert(map_size >= 1 && map_size <= MAX_MAP_SIZE);
            map_size = mapSize;
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        FractalColoring(int maxIterations, int exponent, long double bailout, const Gradient& gradient) :
            FractalColoring(exponent, bailout)
        {
            m_gradient = gradient;
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        FractalColoring(int maxIterations, int exponent, long double bailout, Gradient&& gradient) :
            FractalColoring(exponent, bailout)
        {
            m_gradient = move(gradient);
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        FractalColoring(int maxIterations, int exponent, long double bailout, int mapSize, const Gradient& gradient) : 
            FractalColoring(exponent, bailout)
        {
            assert(map_size >= 1 && map_size <= MAX_MAP_SIZE);
            m_gradient = gradient;
            map_size = mapSize;
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        FractalColoring(int maxIterations, int exponent, long double bailout, int mapSize, Gradient&& gradient) : 
            FractalColoring(exponent, bailout)
        {
            assert(map_size >= 1 && map_size <= MAX_MAP_SIZE);
            m_gradient = move(gradient);
            map_size = mapSize;
            max_iterations = maxIterations;
            color_map = m_gradient.generateGradientMap(map_size);
        }

        void setGradient(const Gradient &gradient){
            m_gradient = gradient;
            color_map = m_gradient.generateGradientMap(map_size);
        }
        void setGradient(Gradient &&gradient){
            m_gradient = move(gradient);
            color_map = m_gradient.generateGradientMap(map_size);
        }

        int getColorMapSize() const noexcept{
            return map_size;
        }

        void setColorMapSize(int mapSize){
            assert(mapSize >= 1 && mapSize <= MAX_MAP_SIZE);
            if(map_size != mapSize){
                map_size = mapSize;
                color_map = m_gradient.generateGradientMap(map_size);
            }
        }

        virtual RGB getPixel(const pair <int, tuple <Complex, Complex, Complex> > &iterOrbit) = 0;

    protected:
        long double m_bailout;
        int max_iterations;
        int m_exponent;
        int map_size = DEFAULT_COLOR_MAP_SIZE;
        Gradient m_gradient = Gradient::WHITE_BLACK;
        vector <RGB> color_map;
};

#endif // FRACTAL_COLORING