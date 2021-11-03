#include "Fractal.hpp"
#include <cstring>

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
    if(width == m_width && height == m_height) return;
    if(width <= 0 or height <= 0){
        cerr << "Can't resize. Dimensions have to be positive integers. Size not changed" << endl;
        return;
    }
    isGenerated = false;
    m_pixels.release();
    m_width = width;
    m_height = height;
    cout << m_width << " " << m_height << endl;
    m_pixels = make_unique<uint8_t[]>(m_width * m_height * 3);
    memset(m_pixels.get(), 0, m_width * m_height * 3);
    setScale(scale->getMinReal(), scale->getMaxReal(), scale->getMinImag(), scale->getMaxImag());
}

void Fractal::setIterations(int iters){
    isGenerated = !falg->setMaxIterationsNumber(iters);
}

void Fractal::setAlgorithm(unique_ptr<FractalAlgorithm> alg){
    if(alg) falg = move(alg);
    else    cerr << "Given algorithm is nullpointer. Not changed" << endl;
}

void Fractal::setScale(long double minR, long double maxR, long double minI, long double maxI){
    bool sameScales = CompareDoubles::isEqual(minR, scale->getMinReal()) and 
                      CompareDoubles::isEqual(maxR, scale->getMaxReal()) and
                      CompareDoubles::isEqual(minI, scale->getMinImag()) and
                      CompareDoubles::isEqual(maxI, scale->getMaxImag());
    if(!sameScales){
        try{
            unique_ptr<Scale> ps = make_unique<Scale>(m_width, m_height, minR, maxR, minI, maxI);
            scale = move(ps);
            isGenerated = false;
        }
        catch(const invalid_argument &invArgument){
            cerr << invArgument.what() << endl;
        }
    }
}

void Fractal::setGradientMapSize(int mapSize){
    isGenerated = !fcol->setColorMapSize(mapSize);
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