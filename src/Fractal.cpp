#include "Fractal.hpp"

// Abbreviation for better readability
using FAT = FractalAlgorithmType;

// Dimension of the fractal plane (minReal, maxReal, minImag, maxImag)
using Dim = tuple <long double, long double, long double, long double>;

const Dim DefaultMandelbrotDim = Dim(-2.2L, 1.0L, -1.2L, 1.2L);
const Dim DefaultMultibrotDim  = Dim(-2.2L, 2.2L, -1.65L, 1.65L);
const Dim DefaultJuliaDim      = Dim(-2.0L, 2.0L, -1.5L, 1.5L);
const Dim DefaultNewtonDim     = Dim(-2.8L, 2.8L, -2.1L, 2.1L);
const Dim DefaultPolyJuliaDim  = DefaultMultibrotDim;
const Dim DefaultNovaDim       = DefaultJuliaDim;

// Fractal
Fractal::Fractal(int width, int height) : Image(width, height) { }


void Fractal::resize(int width, int height){

}

void Fractal::setIterations(int iters){

}

void Fractal::setAlgorithm(unique_ptr<FractalAlgorithm> alg){

}

void Fractal::setScale(long double minR, long double maxR, long double minI, long double maxI){

}

void Fractal::setGradientMapSize(int mapSize){

}

void Fractal::zoom(long long times){

}

void Fractal::unzoom(long long times){
    
}

void Fractal::generate(){
    if(isGenerated) return;
    for(int i = 1; i <= m_width; i++){
        for(int j = 1; j <= m_height; j++){
            auto result = scale->to_scale(i, j);
            Complex c(result.first, result.second);
            setPixel(i, j, fcol->getPixel(falg->getIterationsAndOrbit(c)));
        }
    }
    isGenerated = true;
}

bool Fractal::write(const string &filename, const SaveFormat &format){
    if(!isGenerated) generate();
    Image::write(filename, format);
}

// FractalBuilder
FractalBuilder::FractalBuilder(int width, int height) : fractal(new Fractal(width, height)) { }

FractalBuilder & FractalBuilder::setScale(long double minR, long double maxR, long double minI, long double maxI){
    try{
        unique_ptr<Scale> ps = make_unique<Scale>(fractal->m_width, fractal->m_height, minR, maxR, minI, maxI);
        fractal->scale = move(ps);
    }
    catch(const invalid_argument &invArgument){
        cerr << invArgument.what() << endl;
    }
    return *this;
}

FractalBuilder & FractalBuilder::setAlgorithm(unique_ptr<FractalAlgorithm> alg){
    fractal->falg = move(alg);
    return *this;
}

FractalBuilder & FractalBuilder::setMaxIterations(int mxIter){
    maxIterations = mxIter;
    return *this;
}

FractalBuilder & FractalBuilder::setGradientMapSize(int size){
    if(mapSize >= 1 && mapSize <= MAX_MAP_SIZE)
        mapSize = size;
    return *this;
}

unique_ptr<Fractal> FractalBuilder::build(){
    if(fractal->falg == nullptr){
        fractal->falg = FractalAlgorithmCreator::createMandelbrot();
    }
    fractal->falg->setMaxIterationsNumber(maxIterations);
    FAT algType = fractal->falg->getAlgorithmType();
    bool isConvergence = (algType == FAT::NEWTON) or (algType == FAT::NOVA);
    if(isConvergence){
        fractal->fcol = FractalColoringCreator::createSmoothConvergence(
                            fractal->falg->getMaxIterationsNumber(), fractal->falg->getExponent(),
                            mapSize, move(gradient));
    }
    else{
        fractal->fcol = FractalColoringCreator::createSmoothDivergence(
                            fractal->falg->getMaxIterationsNumber(), fractal->falg->getExponent(),
                            mapSize, move(gradient));
    }
    if(fractal->scale == nullptr){
        long double mr, Mr, mi, Mi;
        switch(algType){
            case FAT::MANDELBROT:
                tie(mr, Mr, mi, Mi) = DefaultMandelbrotDim;
                break;
            case FAT::MULTIBROT:
                tie(mr, Mr, mi, Mi) = DefaultMultibrotDim;
                break;
            case FAT::JULIA:
                tie(mr, Mr, mi, Mi) = DefaultJuliaDim;
                break;
            case FAT::NEWTON:
                tie(mr, Mr, mi, Mi) = DefaultNewtonDim;
                break;
            case FAT::POLYJULIA:
                tie(mr, Mr, mi, Mi) = DefaultPolyJuliaDim;
                break;
            default:
                tie(mr, Mr, mi, Mi) = DefaultNovaDim;
        }
        fractal->scale = make_unique<Scale>(fractal->m_width, fractal->m_height, mr, Mr, mi, Mi);
    }
    return move(fractal);
}