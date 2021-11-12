#ifndef FRACTAL_HEADER
#define FRACTAL_HEADER

#include "FractalColoringCreator.hpp"
#include "FractalAlgorithmCreator.hpp"
#include "Scale.hpp"

/**
 * @brief All stuff related with image processing
 * 
 */
namespace PekiProcessing
{

class FractalBuilder;

/**
 * @brief Abbreviation of FractalAlgorithmType
 * 
 */
using FAT = FractalAlgorithmType;

/**
 * @brief Dimension of the fractal plane (minReal, maxReal, minImag, maxImag)
 * 
 */
using Dim = tuple <long double, long double, long double, long double>;

/**
 * @brief Generating fractal images
 * 
 */
class Fractal : public Image
{
    friend class FractalBuilder;
    public:
        /**
         * @brief Resizing fractal image
         * 
         * @param width - desired width of new fractal image
         * @param height - desired height of new fractal image
         * @return true when successfully resized image
         * @return false otherwise
         */
        bool resize(int width, int height);
        
        
        /**
         * @brief Zoom fractal into area(positive) defined by 2 vertices of rectangle lying on the same diagonal
         *
         * @param v1 - first vertice of rectangle (x, y) where 1 <= x <= width and 1 <= y <= height
         * @param v2 - second vertice of rectangle meeting the same conditions as first
         * @return true when succesfully zoomed image
         * @return false otherwise
         */
        bool setRectangle(pair <int, int> v1, pair<int, int> v2);
        
        /**
         * @brief Set real values range and imaginary values range
         * 
         * @param minR - minimal real number 
         * @param maxR - maximum real number
         * @param minI - minimum imaginary number
         * @param maxI - maximium imaginary number
         * @param baseChanged true when size of fractal was changed(width or height), default setted to false
         * @return true when succsessfully setted scale
         * @return false otherwise
         */
        bool setScale(long double minR, long double maxR, long double minI, long double maxI, bool baseChanged = false);
        
        /**
         * @brief Checks if there is previous scale on scale stack
         * 
         * @return true if exist previous scale
         * @return false otherwise
         */
        bool isPreviousScale();

        /**
         * @brief Saving file
         * 
         * @param filename - path where file is goint to save
         * @param format - save format(e.g. PNG, JPG, GIFF)
         * @return true when successfully saved the file
         * @return false otherwise
         */
        bool write(const string &filename, const SaveFormat &format = SaveFormat::JPEG) override;

        /**
         * @brief Set scale of fractal to previous in stack
         * 
         */
        void setPreviousScale();

        /**
         * @brief Set scale of fractal to default
         * 
         */
        void setDefaultScale();

        /**
         * @brief Set the maximum number of iterations can be done by fractal algorithm
         * 
         * @param iters - number of iterations
         */
        void setIterations(int iters);

        /**
         * @brief Set the algorithm using to generate fractal image
         * 
         * @param alg - pointer to fractal generation algorithm
         * 
         */
        void setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        
        /**
         * @brief Set gradient map size(gradient accuracy)
         * 
         * @param mapSize - size of map
         */
        void setGradientMapSize(int mapSize);
        
        /**
         * @brief Set gradient using to coloring image
         * 
         * @tparam T - the type of gradient, #PekiProcessing::Gradient class has to be constructible from object of this type
         * @param gradient - used to coloring of image
         */
        template <typename T>
        void setGradient(T&& gradient){
            static_assert(is_constructible<Gradient, T>::value, "Cannot construct Gradient object from this type");
            isGenerated = false;
            fcol->setGradient(make_unique<Gradient>(forward<T>(gradient)));
        }

    private:
        /**
         * @brief Construct a new Fractal object. Used only by #FractalBuilder.
         * 
         * @param width - width of the fractal image
         * @param height - height of the fractal image
         */
        Fractal(int width, int height);

        /**
         * @brief Denote if the fractal was already generated
         * 
         */
        bool isGenerated = false;

        /**
         * @brief Scale stack to perform zooming/unzooming operations
         * 
         */
        vector <Dim> scaleStack;

        /**
         * @brief Pointer to the object of class inheriting from #FractalAlgorithm
         * 
         */
        unique_ptr<FractalAlgorithm> falg  = nullptr;
        
        /**
         * @brief Pointer to the object of class inheriting from #FractalColoring
         * 
         */
        unique_ptr<FractalColoring>  fcol  = nullptr;

        /**
         * @brief Pointer to the #Scale object.
         * 
         */
        unique_ptr<Scale>            scale = nullptr;

        /**
         * @brief Generating fractal image 
         * 
         */
        void generate();

        /**
         * @brief Default dimensions of complex plane for Mandelbrot fractal 
         * 
         */
        static constexpr Dim DefaultMandelbrotDim = Dim(-2.2L, 1.0L, -1.2L, 1.2L);

        /**
         * @brief Default dimensions of complex plane for Multibrot fractal 
         * 
         */
        static constexpr Dim DefaultMultibrotDim  = Dim(-2.2L, 2.2L, -1.65L, 1.65L);

        /**
         * @brief Default dimensions of complex plane for Julia fractal 
         * 
         */
        static constexpr Dim DefaultJuliaDim      = Dim(-2.0L, 2.0L, -1.5L, 1.5L);
        
        /**
         * @brief Default dimensions of complex plane for Newton fractal 
         * 
         */
        static constexpr Dim DefaultNewtonDim     = Dim(-2.8L, 2.8L, -2.1L, 2.1L);
        
        /**
         * @brief Default dimensions of complex plane for PolyJuliaSet fractal 
         * 
         */
        static constexpr Dim DefaultPolyJuliaDim  = DefaultMultibrotDim;
        
        /**
         * @brief Default dimensions of complex plane for Nova fractal 
         * 
         */
        static constexpr Dim DefaultNovaDim       = DefaultJuliaDim;
        
        /**
         * @brief Simple mapping of default dimensions based on #FractalAlgorithmType
         * 
         * @param alg - fractal algorithm type 
         * @return - one of the default dimensions depending on function argument
         */
        static Dim getDefaultDimension(const FAT alg) noexcept;
};

/**
 * @brief Builder of Fractal class
 * 
 */
class FractalBuilder
{
    public:
        /**
         * @brief Construct a new #FractalBuilder object
         * 
         * @param width fractal width
         * @param height fractal height
         */
        FractalBuilder(int width, int height);

        /**
         * @brief Real and imaginary values ranges used to initialize #Fractal instance
         * 
         * @param minR - minimal real number 
         * @param maxR - maximum real number
         * @param minI - minimum imaginary number
         * @param maxI - maximium imaginary number
         * @return reference to itself
         */
        FractalBuilder & setScale(long double minR, long double maxR, long double minI, long double maxI);
        
        /**
         * @brief Set the algorithm used to initialize #Fractal instance
         * 
         * @param alg - pointer to fractal generation algorithm
         * 
         * @return reference to itself
         */
        FractalBuilder & setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        
        /**
         * @brief Set the max iterations number used to initialize #Fractal instance
         * 
         * @param mxIter - max iterations number 
         * @return reference to itself
         */
        FractalBuilder & setMaxIterations(int mxIter);

        /**
         * @brief Set the gradient map size used to initialize #Fractal instance
         * 
         * @param size - gradient map size(accuracy)
         * @return reference to itself
         */
        FractalBuilder & setGradientMapSize(int size);

        /**
         * @brief Create instance of #Fractal class
         * 
         * @return pointer to new instance of #Fractal class
         */
        unique_ptr<Fractal> build();

        /**
         * @brief Set gradient used to initialize #Fractal instance
         * 
         * @tparam T - the type of gradient, #PekiProcessing::Gradient class has to be constructible from object of this type
         * @param gradient - used to coloring of image
         * @return reference to itself
         */
        template<typename T>
        FractalBuilder & setGradient(T&& g){
            static_assert(is_constructible<Gradient, T>::value, "Cannot construct Gradient object from this type");
            gradient = make_unique<Gradient>(forward<T>(g));
            return *this;
        }

    private:
        int maxIterations = -1;
        int mapSize = -1;
        unique_ptr<Gradient> gradient = nullptr;
        unique_ptr<Fractal> fractal = nullptr;
};

} // Namespace end
#endif // FRACTAL_HEADER