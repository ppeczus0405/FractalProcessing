#ifndef FRACTAL_HEADER
#define FRACTAL_HEADER

#include "FractalColoringCreator.hpp"
#include "FractalAlgorithmCreator.hpp"
#include "Scale.hpp"

class FractalBuilder;

class Fractal : public Image
{
    friend class FractalBuilder;
    public:
        void resize(int width, int height);
        void setIterations(int iters);
        void setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        void setScale(long double minR, long double maxR, long double minI, long double maxI);
        void setGradientMapSize(int mapSize);
        void zoom(long long times);
        void unzoom(long long times);
        bool write(const string &filename, const SaveFormat &format = SaveFormat::JPEG) override;
        
        template <typename T>
        void setGradient(T&& gradient){

        }

    private:
        Fractal(int width, int height);
        bool isGenerated = false;
        unique_ptr<FractalAlgorithm> falg  = nullptr;
        unique_ptr<FractalColoring>  fcol  = nullptr;
        unique_ptr<Scale>            scale = nullptr;

        void generate();
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
        int maxIterations = DEFAULT_ITERATIONS;
        int mapSize = DEFAULT_COLOR_MAP_SIZE;
        unique_ptr<Gradient> gradient = nullptr;
        unique_ptr<Fractal> fractal = nullptr;
};

#endif // FRACTAL_HEADER