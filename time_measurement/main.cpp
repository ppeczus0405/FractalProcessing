#include <cassert>
#include <ios>
#include <iostream>
#include <format>
#include <fstream>
#include <vector>
#include <chrono>
#include "Fractal.hpp"
#include "FractalAlgorithmCreator.hpp"
#include "RGB.hpp"

constexpr std::string_view path{"/home/p3ki/Dev/eng_degree/FractalProcessing/time_measurement/results"};

std::string fractalType;
std::string mode;
int width = 0, height = 0, iterations = 0;
int timeMs = -1;
std::string outputFile;
std::string singleResultFile;
std::string singleImageFile;

namespace {

void printUsage() {
    std::cerr << "Usage: ./fractal_benchmark --fractal mandelbrot|julia|newton "
                 "--mode cpu|gpu --width <int> --height <int> "
                 "--iterations <int>\n";
}

void dumpArguments()
{
  std::cout << "fractalType: " << fractalType << std::endl;
  std::cout << "mode: " << mode << std::endl;
  std::cout << "resolution: " << width << "x" << height << std::endl;
  std::cout << "iterations: " << iterations << std::endl;
  singleResultFile = std::format("{}/single/{}-{}-{}x{}-{}.csv", path, fractalType, mode, width, height, iterations);
  singleImageFile = std::format("{}/single/{}-{}-{}x{}-{}-iteration_bounds", path, fractalType, mode, width, height, iterations);
  outputFile = std::format("{}/result.csv", path);
  //std::cout << "singleResultFile: " << singleResultFile << std::endl;
  std::cout << "singleImageFile: " << singleImageFile << ".jpg" << std::endl;
  //std::cout << "outputFile: " << outputFile << std::endl;
}

void saveResultsToFile(std::string filename) {
    // Zapis do pliku CSV
    bool writeHeader = false;
    std::ifstream testExist(filename);
    if (!testExist.good()) writeHeader = true;

    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cerr << "Failed to open output file: " << outputFile << "\n";
    }

    if (writeHeader) {
        out << "tryb,fraktal,rozdzielczosc,iteracje,czas_ms\n";
    }

    out << (mode == "gpu" ? "GPU" : "CPU") << ","
        << fractalType << ","
        << width << "x" << height << ","
        << iterations << ","
        << timeMs << "\n";
}

std::vector<std::pair<double, PekiProc::RGB>>
getDefaultGradient() {
  return {
      {0.0, {0, 0, 0}},           // Black
      {0.1, {0, 100, 200}},       // Cool Blue
      {0.5, {255, 165, 0}},       // Orange
      {1.0, {255, 255, 255}}      // White
  };
}

std::unique_ptr<PekiProc::FractalAlgorithm>
createAlgorithm() {
  using namespace PekiProc;

  if(fractalType == "mandelbrot") {
    return FractalAlgorithmCreator::createMandelbrot();
  }
  else if(fractalType == "julia") {
    return FractalAlgorithmCreator::createJuliaSet({-0.7, 0.27015});
  }
  else if(fractalType == "newton") {
    std::vector<PekiProc::Complex> polynomialTerms(4);
    polynomialTerms[0] = -Complex::ONE();
    polynomialTerms[1] = Complex::ZERO();
    polynomialTerms[2] = Complex::ZERO();
    polynomialTerms[3] = Complex::ONE();
    Complex relaxation = Complex::ONE();
    return FractalAlgorithmCreator::createNewton(polynomialTerms, relaxation);
  }
  else
    assert(!"Not known algorithm.");
  return {};
}

std::unique_ptr<PekiProc::Fractal> createFractal()
{
  bool gpu_on = (mode == "gpu") || (mode == "GPU");

  if(gpu_on) {
    std::cout << "GPU Fractal generation" << std::endl;
    return PekiProc::FractalBuilder(width, height)
                  .setMaxIterations(iterations)
                  .setGradient(getDefaultGradient())
                  .setGradientMapSize(1001)
                  .setAlgorithm(createAlgorithm())
                  .setGpuAcceleration()
                  .build();
  }
  std::cout << "CPU Fractal generation" << std::endl;
  return PekiProc::FractalBuilder(width, height)
                  .setMaxIterations(iterations)
                  .setGradient(getDefaultGradient())
                  .setGradientMapSize(1001)
                  .setAlgorithm(createAlgorithm())
                  .build();
}

void benchmark() {
    auto fractal = createFractal();
    fractal->write(singleImageFile);
}

} // anonymous namespace


int main(int argc, char* argv[]) {
  if (argc != 11) {
      printUsage();
      return 1;
  }

  for (int i = 1; i < argc; i += 2) {
      std::string arg = argv[i];
      if (arg == "--fractal") {
          fractalType = argv[i + 1];
      } else if (arg == "--mode") {
          mode = argv[i + 1];
      } else if (arg == "--width") {
          width = std::stoi(argv[i + 1]);
      } else if (arg == "--height") {
          height = std::stoi(argv[i + 1]);
      } else if (arg == "--iterations") {
          iterations = std::stoi(argv[i + 1]);
      } else {
          printUsage();
          return 1;
      }
  }

  dumpArguments();
  benchmark();
  //saveResultsToFile(singleResultFile);
  //saveResultsToFile(outputFile);
  return 0;
}
