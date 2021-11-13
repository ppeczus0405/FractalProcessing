#ifndef RGB_HEADER
#define RGB_HEADER

#include <iostream>
#include <cstdint>
using namespace std;

/**
 * @brief Representation of color
 * 
 */
class RGB
{
    public:
        /**
         * @brief Construct a RGB object with black color inside.
         * 
         */
        RGB() : r(0), g(0), b(0) { }

        /**
         * @brief Construct a new RGB object with color specified by arguments.
         * 
         * @param R - red color value,   0 <= R <= 255
         * @param G - green color value, 0 <= G <= 255
         * @param B - blue color value,  0 <= B <= 255
         */
        RGB(uint8_t R, uint8_t G, uint8_t B) : r(R), g(G), b(B) { }
        
        const static RGB WHITE, GRAY, BLACK, RED, GREEN, BLUE;

        /**
         * @brief Get red value
         * 
         * @return red value 
         */
        uint8_t getR() const{
            return r;
        }

        /**
         * @brief Get green value
         * 
         * @return green value 
         */
        uint8_t getG() const{
            return g;
        }

        /**
         * @brief Get blue value
         * 
         * @return blue value 
         */
        uint8_t getB() const{
            return b;
        }

        /**
         * @brief Set value of red color
         * 
         * @param c - red color value, 0 <= c <= 255 
         */
        void setR(uint8_t c){
            r = c;
        }

        /**
         * @brief Set value of green color
         * 
         * @param c - green color value, 0 <= c <= 255 
         */
        void setG(uint8_t c){
            g = c;
        }

        /**
         * @brief Set value of blue color
         * 
         * @param c - blue color value, 0 <= c <= 255 
         */
        void setB(uint8_t c){
            b = c;
        }

        /**
         * @brief Overloaded output stream. Redirect to the output stream in format: "RGB(color.r, color.g, color.b)" 
         */
        friend ostream& operator<<(ostream & stream, const RGB &color){
            stream << "RGB(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")";
            return stream;
        }

    private:
        uint8_t r, g, b;
};

#endif // RGB_HEADER