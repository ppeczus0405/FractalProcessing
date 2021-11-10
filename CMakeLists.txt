cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Wshadow -pedantic")

project(FractalGenerator)
find_package(ImageMagick COMPONENTS Magick++)


set(HEADERS_DIR src)
set(SOURCE_DIR src)

set(SOURCE ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/Complex.cpp ${SOURCE_DIR}/Mandelbrot.cpp 
           ${SOURCE_DIR}/Gradient.cpp ${SOURCE_DIR}/ImageSaver.cpp ${SOURCE_DIR}/Image.cpp
           ${SOURCE_DIR}/JuliaSet.cpp ${SOURCE_DIR}/Newton.cpp ${SOURCE_DIR}/Fractal.cpp)

set(HEADERS ${HEADERS_DIR}/Complex.hpp ${HEADERS_DIR}/CompareDoubles.hpp ${HEADERS_DIR}/Mandelbrot.hpp
            ${HEADERS_DIR}/Scale.hpp ${HEADERS_DIR}/spline.hpp ${HEADERS_DIR}/RGB.hpp ${HEADERS_DIR}/Gradient.hpp
            ${HEADERS_DIR}/ImageSaver.hpp ${HEADERS_DIR}/Image.hpp ${SOURCE_DIR}/JuliaSet.hpp 
            ${HEADERS_DIR}/FractalAlgorithm.hpp ${HEADERS_DIR}/FractalAlgorithmCreator.hpp ${HEADERS_DIR}/Newton.hpp
            ${HEADERS_DIR}/FractalColoring.hpp ${HEADERS_DIR}/FractalColoringCreator.hpp ${HEADERS_DIR}/SmoothConvergence.hpp
            ${HEADERS_DIR}/SmoothDivergence.hpp ${HEADERS_DIR}/Fractal.hpp)

SET(PROJECT_SOURCES ${HEADERS} ${SOURCE})

add_executable(fractalgen ${PROJECT_SOURCES})
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
target_include_directories(fractalgen PRIVATE ${HEADERS_DIR} ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(fractalgen ${ImageMagick_LIBRARIES})
set_target_properties(fractalgen PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}//bin")