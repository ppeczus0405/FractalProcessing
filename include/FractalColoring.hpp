#ifndef FRACTAL_COLORING
#define FRACTAL_COLORING

#include "Gradient.hpp"
#include "RGB.hpp"
#include "Complex.hpp"
using namespace std;
using namespace PekiProcessing;

/**
 * @brief Default size of gradient color map(accuracy of the gradient)
 * 
 */
constexpr int DEFAULT_COLOR_MAP_SIZE = 512;

/**
 * @brief Maximum size of gradient color map(accuracy of the gradient)
 * 
 */
constexpr int MAX_MAP_SIZE = 16384;

/**
 * @brief Minimum size of gradient color map(accuracy of the gradient)
 * 
 */
constexpr int MIN_MAP_SIZE = 1;

/**
 * @brief Abstract class used as a base of different fractal coloring classes
 * 
 */
class FractalColoring{
    private:
        /**
         * @brief Initialize exponent
         * 
         * @param exponent - highest power of variable in polynomial used to generate fractal image
         */
        FractalColoring(int exponent) : 
            m_exponent(exponent) {}
        
        /**
         * @brief Initialize exponent and bailout
         * 
         * @param exponent - highest power of variable in polynomial used to generate fractal image
         * @param bailout - the bound of the orbit trap
         */
        FractalColoring(int exponent, long double bailout) :  FractalColoring(exponent) 
            {
                m_bailout = bailout;
            }

    public:

        /**
         * @brief Main constructor used to initialize all members
         * 
         * @param maxIterations - maximal number of iterations that fractal algorithm can perform
         * @param exponent - highest power of variable in polynomial used to generate fractal image
         * @param bailout - the bound of the orbit trap
         * @param mapSize - size of the gradient color map
         * @param gradient - object used to color fractal image
         * @note In case where gradient = nullptr, initializing with #PekiProcessing::Gradient::WHITE_BLACK
         */
        FractalColoring(int maxIterations, int exponent, long double bailout, 
                        int mapSize = DEFAULT_COLOR_MAP_SIZE, unique_ptr<Gradient> gradient = nullptr)
                            : FractalColoring(exponent, bailout)
        {
            if(gradient) m_gradient = move(gradient);
            else         m_gradient = make_unique<Gradient>(Gradient::WHITE_BLACK);

            setColorMapSize(mapSize);
            max_iterations = maxIterations;
        }

        /**
         * @brief Set the gradient if it's possible and generate new gradient map
         *  
         * 
         * @param gradient - pointer to the gradient object
         * @note In case where gradient = nullptr, initializing with #PekiProcessing::Gradient::WHITE_BLACK
         */
        void setGradient(unique_ptr<Gradient> gradient){
            if(gradient){
                m_gradient = move(gradient);
                color_map = m_gradient->generateGradientMap(map_size);
            }
        }

        /**
         * @brief Changing owner of gradient
         * 
         * @return moved gradient pointer
         */
        unique_ptr<Gradient> getGradient() noexcept{
            return move(m_gradient);
        }

        /**
         * @brief Getting size of gradient color map(accuracy of the gradient)
         * 
         * @return gradient map color size
         */
        int getColorMapSize() const noexcept{
            return map_size;
        }

        /**
         * @brief Set the gradient map size and generate gradient map with new size
         * 
         * @details The diagnostic messages are printed in case when problem with new map size was occurred
         * 
         * @param mapSize - new map size
         * @return true if new gradient map was generated
         * @return false if new gradient map size is the same or not proper(mapSize < #MIN_MAP_SIZE or mapSize > #MAX_MAP_SIZE)
         */
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

        /**
         * @brief Set the maximum iterations number.
         * 
         * @details Without restrictions due to usage only with proper iterations number derived from fractal algorithm object
         *  
         * @param iters - maximum number of iterations
         */
        void setMaxIterations(int iters){
            max_iterations = iters;
        }

        /**
         * @brief Get final pixel used in the image
         * 
         * 
         * @param iterOrbit - iterations number and orbit. It's exactly result of FractalAlgorithm::getIterationsAndOrbit(const Complex &c) 
         * @return RGB object - final pixel in the fractal image
         */ 
        virtual RGB getPixel(const pair <int, tuple <Complex, Complex, Complex> > &iterOrbit) = 0;

    protected:
        /**
         * @brief the bound of the orbit trap
         * 
         */
        long double m_bailout;
        
        /**
         * @brief It is #FractalAlgorithm::max_iter. Used also to coloring.
         * 
         */
        int max_iterations;
        
        /**
         * @brief highest power of variable in polynomial used to generate fractal image
         * 
         */
        int m_exponent;

        /**
         * @brief - size of the gradient color map
         * 
         */
        int map_size = -1;

        /**
         * @brief pointer to the gradient object used to color fractal image
         * 
         */
        unique_ptr<Gradient> m_gradient = nullptr;

        /**
         * @brief gradient map - result of PekiProcessing::Gradient::generateGradientMap(int)
         * 
         */
        vector <RGB> color_map;
};

#endif // FRACTAL_COLORING