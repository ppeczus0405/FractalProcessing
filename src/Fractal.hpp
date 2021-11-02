#ifndef FRACTAL_HEADER
#define FRACTAL_HEADER

#include "FractalColoring.hpp"
#include "FractalAlgorithm.hpp"
#include "Scale.hpp"

class FractalBuilder;

class Fractal : public Image
{
    friend class FractalBuilder;
    public:
        ~Fractal() override;
        template <typename T>
        void setGradient(T&& gradient);
        void resize(int width, int height);
        void setIterations(int iters);
        void setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        void setScale(long double minR, long double maxR, long double minI, long double maxI);
        void zoom(long long times);
        void unzoom(long long times);
    private:
        Fractal(int width, int height);
        bool is_generated = false;
        unique_ptr<FractalAlgorithm> falg = nullptr;
        unique_ptr<FractalColoring>  fcol = nullptr;
        unique_ptr<Scale>            s    = nullptr;
};


class FractalBuilder
{
    public:
        FractalBuilder(int width, int height);
        FractalBuilder & setScale(long double minR, long double maxR, long double minI, long double maxI);
        FractalBuilder & setAlgorithm(unique_ptr<FractalAlgorithm> alg);
        FractalBuilder & setMaxIterations(int mxIter);
        template<typename T>
        FractalBuilder & setGradient(T&& gradient);
        unique_ptr<Fractal> build();
    private:
        unique_ptr<Fractal> fractal = nullptr;
};

#endif // FRACTAL_HEADER