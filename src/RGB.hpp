#ifndef RGB_HEADER
#define RGB_HEADER

#include <iostream>
#include <cstdint>
using namespace std;

class RGB
{
    public:
        RGB(uint8_t R, uint8_t G, uint8_t B) : r(R), g(G), b(B) { }
        
        uint8_t getR() const{
            return r;
        }
        uint8_t getG() const{
            return g;
        }
        uint8_t getB() const{
            return b;
        }

        void setR(uint8_t c){
            r = c;
        }
        void setG(uint8_t c){
            g = c;
        }
        void setB(uint8_t c){
            b = c;
        }

        friend ostream& operator<<(ostream & stream, const RGB &color){
            stream << "RGB(" << color.r << ", " << color.g << ", " << color.b << ")";
            return stream;
        }

    private:
        uint8_t r, g, b;
};

#endif // RGB_HEADER