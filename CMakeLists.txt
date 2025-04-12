cmake_minimum_required(VERSION 3.21)

option(BUILD_SAMPLE_APP "Build QT sample app." ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(FractalProcessing LANGUAGES CXX CUDA)
find_package(ImageMagick COMPONENTS Magick++)

set(HEADERS_DIR include)
set(SOURCE_DIR src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/$<CONFIG>/lib")

set(SOURCE ${SOURCE_DIR}/Complex.cpp ${SOURCE_DIR}/Mandelbrot.cpp 
           ${SOURCE_DIR}/Gradient.cpp ${SOURCE_DIR}/ImageSaver.cpp ${SOURCE_DIR}/Image.cpp
           ${SOURCE_DIR}/JuliaSet.cpp ${SOURCE_DIR}/Newton.cpp ${SOURCE_DIR}/Fractal.cpp)

add_library(fractalproc SHARED ${SOURCE})
target_compile_features(fractalproc PUBLIC cxx_std_17)
set_target_properties(fractalproc PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
target_include_directories(fractalproc PUBLIC ${HEADERS_DIR} PRIVATE ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(fractalproc PUBLIC ${ImageMagick_LIBRARIES})

if(BUILD_SAMPLE_APP)
    add_subdirectory(sample)
endif()
