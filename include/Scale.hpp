#ifndef SCALING_HEADER
#define SCALING_HEADER

#include <utility>
#include "CompareDoubles.hpp"
using namespace std;

/**
 * @brief Scales pixels coordinates into complex plane coordinates
 * 
 */
class Scale
{
    public:
        /**
         * @brief Construct a new Scale object
         * 
         * @param baseW - width of the image 
         * @param baseH - height of the image
         * @param widthm - minimum real part value
         * @param widthM - maximum real part value
         * @param heightm - minimum imaginary part value
         * @param heightM - maximum imaginary part value
         * @throw invalid_argument in cases when:\n
         *        - widthm > widhtM
         *        - heightm > heightM
         *        - baseWidth < 1 or baseHeight < 1
         */
        Scale(int baseW, int baseH, long double widthm, long double widthM, long double heightm, long double heightM)
             : baseWidth(baseW), baseHeight(baseH),
               widthMin(widthm), widthMax(widthM),
               heightMin(heightm), heightMax(heightM)
        { 
            if(CompareDoubles::isGreater(widthm, widthM)){
                throw invalid_argument("You can't scale image. Minimal width > Maximal width");
            } // width_min > width_max
            if(CompareDoubles::isGreater(heightm, heightM)){
                throw invalid_argument("You can't scale image. Minimal height > Maximal height");
            } // height_min > height_max
            if(baseWidth < 1 || baseHeight < 1){
                throw invalid_argument("One of the base dimensions is less or equal 0");
            } // Invalid image dimensions
            
            // Special case handling - when baseWidth/Height = 1;
            if(baseWidth != 1)  wratio = (widthMax - widthMin) / (baseWidth - 1);
            else                baseWidthOne = true;

            if(baseHeight != 1) hratio = (heightMax - heightMin) / (baseHeight - 1);
            else                baseHeightOne = true;       
        }

        /**
         * @brief Get the scaled point on the complex plane
         * 
         * @param x - pixel coordinate on the OX axis
         * @param y - pixel coordiante on the OY axis
         * @throw invalid_argument in cases when:
         *        - x < 1 or x > baseWidth
         *        - y < 1 or y > baseHeight
         * @return point on the complex plane
         */
        pair <long double, long double> getScaled(int x, int y){
            if(x < 1 || x > baseWidth){
                string error_message = "Error: Can't scale pixel. Reason: " + to_string(x) + "is not in range [1, ";
                error_message.append(to_string(baseWidth) + "]");
                throw invalid_argument(error_message);
            }
            if(y < 1 || y > baseHeight){
                string error_message = "Error: Can't scale pixel. Reason: " + to_string(y) + "is not in range [1, ";
                error_message.append(to_string(baseHeight) + "]");
                throw invalid_argument(error_message);
            }
            
            long double scaledX = (widthMin + widthMax) / 2.0L;
            long double scaledY = (heightMin + heightMax) / 2.0L;
            if(!baseWidthOne)  scaledX = widthMin + (x - 1) * wratio;
            if(!baseHeightOne) scaledY = heightMin + (y - 1) * hratio;
            return {scaledX, scaledY};
        }

        /**
         * @brief Get the minimum real part value
         * 
         * @return minimum real part value
         */
        inline long double getMinReal() {
            return widthMin;
        }
        
        /**
         * @brief Get the maximum real part value
         * 
         * @return maximum real part value
         */
        inline long double getMaxReal() {
            return widthMax;
        }

        /**
         * @brief Get the minimum imaginary part value
         * 
         * @return minimum imaginary part value
         */
        inline long double getMinImag() {
            return heightMin;
        }
        
        /**
         * @brief Get the maximum imaginary part value
         * 
         * @return maximum imaginary part value
         */
        inline long double getMaxImag() {
            return heightMax;
        }
    
    private:
        bool baseWidthOne = false, baseHeightOne = false;
        int baseWidth, baseHeight;
        long double widthMin, widthMax, wratio;
        long double heightMin, heightMax, hratio;
};

#endif // SCALING_HEADER