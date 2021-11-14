cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Wshadow -pedantic")

project(FractalProcessing)
find_package(ImageMagick COMPONENTS Magick++)

set(HEADERS_DIR include)
set(SOURCE_DIR src)

set(SOURCE ${SOURCE_DIR}/Complex.cpp ${SOURCE_DIR}/Mandelbrot.cpp 
           ${SOURCE_DIR}/Gradient.cpp ${SOURCE_DIR}/ImageSaver.cpp ${SOURCE_DIR}/Image.cpp
           ${SOURCE_DIR}/JuliaSet.cpp ${SOURCE_DIR}/Newton.cpp ${SOURCE_DIR}/Fractal.cpp)

add_library(fractalproc ${SOURCE})
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
target_include_directories(fractalproc PRIVATE ${HEADERS_DIR} ${ImageMagick_INCLUDE_DIRS})
set_target_properties(fractalproc PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}//lib")