#ifndef FRACTAL_COLORING
#define FRACTAL_COLORING
 
#include "Gradient.hpp"
#include "RGB.hpp"
#include "Complex.hpp"
using namespace std;
using namespace PekiProcessing;
 
constexpr int DEFAULT_COLOR_MAP_SIZE = 512;
constexpr int MAX_MAP_SIZE = 16384;
constexpr int MIN_MAP_SIZE = 1;

class FractalColoring{ 
    public:
        FractalColoring(int maxIterations, int exponent, long double bailout, 
                        int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
                            : FractalColoring(exponent, bailout)
        {
            if(gradient) m_gradient = move(gradient);
            else         m_gradient = make_unique<Gradient>(Gradient::WHITE_BLACK);
 
            setColorMapSize(mapSize);
            max_iterations = maxIterations;
        }
 
        void setGradient(unique_ptr<Gradient> gradient){
            if(gradient){
                m_gradient = move(gradient);
                color_map = m_gradient->generateGradientMap(map_size);
            }
        }
 
        unique_ptr<Gradient> getGradient() noexcept{
            return move(m_gradient);
        }
 
        int getColorMapSize() const noexcept{
            return map_size;
        }
 
        bool setColorMapSize(int mapSize){
            if(mapSize >= MIN_MAP_SIZE && mapSize <= MAX_MAP_SIZE){
                if(map_size != mapSize){
                    map_size = mapSize;
                    color_map = m_gradient->generateGradientMap(map_size);
                    return true;
                }
                return false;
            }
            else{
                cerr << "Map size has to meet the condition: ";
                cerr << MIN_MAP_SIZE << " <= size <= " << MAX_MAP_SIZE << endl;
                if(map_size == -1){
                    cerr << "Setted map size to " << DEFAULT_COLOR_MAP_SIZE << endl;
                    map_size = DEFAULT_COLOR_MAP_SIZE;
                    color_map = m_gradient->generateGradientMap(map_size);
                    return true;
                }
                else{
                    cerr << "Size not changed" << endl;
                    return false;
                }
            }
        }
 
        void setMaxIterations(int iters){
            max_iterations = iters;
        }
 
        virtual RGB getPixel(const pair <int, tuple <Complex, Complex, Complex> > &iterOrbit) = 0;
 
    protected:
        long double m_bailout;
        
        int max_iterations;
        int m_exponent;
        int map_size = -1;

        unique_ptr<Gradient> m_gradient = nullptr;
        vector <RGB> color_map;
        
    private:
        FractalColoring(int exponent) : 
            m_exponent(exponent) {}
        
        FractalColoring(int exponent, long double bailout) :  FractalColoring(exponent) 
        {
            m_bailout = bailout;
        }
};
 
#endif // FRACTAL_COLORING

