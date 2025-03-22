#ifndef PEKI_FRACTAL_ALGORITHM_CREATOR_HPP
#define PEKI_FRACTAL_ALGORITHM_CREATOR_HPP

#include "FractalAlgorithm.hpp"
#include "JuliaSet.hpp"
#include "Mandelbrot.hpp"
#include "Newton.hpp"

namespace PekiProc {

class FractalAlgorithmCreator {
 public:
  FractalAlgorithmCreator() = delete;

  static std::unique_ptr<FractalAlgorithm> createMandelbrot() {
    return std::make_unique<Mandelbrot>();
  }

  static std::unique_ptr<FractalAlgorithm> createMultibrot(const int exponent) {
    return std::make_unique<Mandelbrot>(exponent);
  }

  static std::unique_ptr<FractalAlgorithm> createJuliaSet() {
    return std::make_unique<JuliaSet>();
  }

  static std::unique_ptr<FractalAlgorithm> createJuliaSet(
      const Complex& incremenation) {
    return std::make_unique<JuliaSet>(incremenation);
  }

  static std::unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(
      const int exponent) {
    return std::make_unique<JuliaSet>(exponent);
  }

  static std::unique_ptr<FractalAlgorithm> createPolynomialJuliaSet(
      const int exponent, const Complex& incrementation) {
    return std::make_unique<JuliaSet>(exponent, incrementation);
  }

  static std::unique_ptr<FractalAlgorithm> createNewton() {
    return std::make_unique<Newton>();
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNewton(T&& polynomial) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial));
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNewton(
      T&& polynomial, const Complex& relaxation) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial), relaxation);
  }

  static std::unique_ptr<FractalAlgorithm> createNova() {
    return std::make_unique<Newton>(true, false);
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNova(T&& polynomial) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial), true, false);
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNova(
      T&& polynomial, const Complex& relaxation) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial), relaxation,
                                    true, false);
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNova(T&& polynomial,
                                                      const Complex& relaxation,
                                                      const Complex& startval) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial), relaxation,
                                    startval);
  }

  template <typename T>
  static std::unique_ptr<FractalAlgorithm> createNova(T&& polynomial,
                                                      const Complex& relaxation,
                                                      const bool pixstart) {
    static_assert(std::is_constructible<Newton, T>::value,
                  "Can't pass given polynomial to Newton class constructor");
    return std::make_unique<Newton>(std::forward<T>(polynomial), relaxation,
                                    true, pixstart);
  }
};

}  // namespace PekiProc

#endif  // PEKI_FRACTAL_ALGORITHM_CREATOR_HPP
