#ifndef SCALING_HEADER
#define SCALING_HEADER

#include <utility>
#include "CompareDoubles.hpp"
using namespace std;

class Scale
{
    private:
        int baseWidth, baseHeight;
        double widthMin, widthMax, wratio;
        double heightMin, heightMax, hratio;
    public:
        Scale(int baseW, int baseH, double widthm, double widthM, double heightm, double heightM)
             : baseWidth(baseW), baseHeight(baseH),
               widthMin(widthm), widthMax(widthM),
               heightMin(heightm), heightMax(heightM)
        { 
            if(CompareDoubles::isGreater(widthm, widthM)){
                throw new invalid_argument("You can't scale image. Minimal width > Maximal width");
            } // width_min > width_max
            if(CompareDoubles::isGreater(heightm, heightM)){
                throw new invalid_argument("You can't scale image. Minimal height > Maximal height");
            } // height_min > height_max
            if(baseWidth < 1 || baseHeight < 1){
                throw new invalid_argument("One of the base dimensions is less or equal 0");
            } // Invalid image dimensions
            if( (baseWidth == 1 && !CompareDoubles::isEqual(widthMin, widthMax)) ||
                (baseHeight == 1 && !CompareDoubles::isEqual(heightMin, heightMax)) ){
                throw new invalid_argument("Can't scale one pixel into range [a, b] where a != b");
            } // One pixel and scale range [a, b] where a != b

            wratio = (widthMax - widthMin) / (baseWidth - 1);
            hratio = (heightMax - heightMin) / (baseHeight - 1);
        }
        
        pair <double, double> to_scale(int x, int y){
            if(x < 1 || x > baseWidth){
                string error_message = "Error: Can't scale pixel. Reason: " + to_string(x) + "is not in range [1, ";
                error_message.append(to_string(baseWidth) + "]");
                throw new invalid_argument(error_message);
            }
            if(y < 1 || y > baseHeight){
                string error_message = "Error: Can't scale pixel. Reason: " + to_string(y) + "is not in range [1, ";
                error_message.append(to_string(baseHeight) + "]");
                throw new invalid_argument(error_message);
            }
            return {widthMin + (x - 1) * wratio, heightMin + (y - 1) * hratio};
        }
};

#endif // SCALING_HEADER