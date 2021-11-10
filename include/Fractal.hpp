#ifndef FRACTAL_HEADER
#define FRACTAL_HEADER

#include "FractalColoringCreator.hpp"
#include "FractalAlgorithmCreator.hpp"
#include "Scale.hpp"

class FractalBuilder;

// Abbreviation for better readability
using FAT = FractalAlgorithmType;

// Dimension of the fractal plane (minReal, maxReal, minImag, maxImag)
using Dim = tuple <long double, long double, long double, long double>;


class Fractal : public Image
{
    friend class FractalBuilder;
    public:
        bool resize(int width, int height);
        // Parameters are any 2 vertices of rectangle(with surface area > 0) placed on diagonal
        bool setRectangle(pair <int, int> v1, pair<int, int> v2);
        bool setScale(long double minR, long double maxR, long double minI, long double maxI, bool baseChanged = false);
        bool isPreviousScale();
        bool write(const string &filename, const SaveFormat &format = SaveFormat::JPEG) override;

        void setPreviousScale();
        void setDefaultScale();
        void setIterations(int iters);
        void setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        void setGradientMapSize(int mapSize);
        
        template <typename T>
        void setGradient(T&& gradient){
            isGenerated = false;
            fcol->setGradient(make_unique<Gradient>(forward<T>(gradient)));
        }

    private:
        Fractal(int width, int height);
        bool isGenerated = false;
        vector <Dim> scaleStack;
        unique_ptr<FractalAlgorithm> falg  = nullptr;
        unique_ptr<FractalColoring>  fcol  = nullptr;
        unique_ptr<Scale>            scale = nullptr;

        void generate();

        // Static part - default diemensions of particular fractals
        static constexpr Dim DefaultMandelbrotDim = Dim(-2.2L, 1.0L, -1.2L, 1.2L);
        static constexpr Dim DefaultMultibrotDim  = Dim(-2.2L, 2.2L, -1.65L, 1.65L);
        static constexpr Dim DefaultJuliaDim      = Dim(-2.0L, 2.0L, -1.5L, 1.5L);
        static constexpr Dim DefaultNewtonDim     = Dim(-2.8L, 2.8L, -2.1L, 2.1L);
        static constexpr Dim DefaultPolyJuliaDim  = DefaultMultibrotDim;
        static constexpr Dim DefaultNovaDim       = DefaultJuliaDim;
        static Dim getDefaultDimension(const FAT alg) noexcept;
};


class FractalBuilder
{
    public:
        FractalBuilder(int width, int height);
        FractalBuilder & setScale(long double minR, long double maxR, long double minI, long double maxI);
        FractalBuilder & setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        FractalBuilder & setMaxIterations(int mxIter);
        FractalBuilder & setGradientMapSize(int size);
        unique_ptr<Fractal> build();

        template<typename T>
        FractalBuilder & setGradient(T&& g){
            gradient = make_unique<Gradient>(forward<T>(g));
            return *this;
        }

    private:
        int maxIterations = -1;
        int mapSize = -1;
        unique_ptr<Gradient> gradient = nullptr;
        unique_ptr<Fractal> fractal = nullptr;
};

#endif // FRACTAL_HEADER