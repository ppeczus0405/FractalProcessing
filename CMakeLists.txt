cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Wshadow -pedantic")

project(FractalGenerator)

INCLUDE(FindPkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

set(HEADERS_DIR src)
set(SOURCE_DIR src)

set(SOURCE ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/Complex.cpp ${SOURCE_DIR}/Bitmap.cpp ${SOURCE_DIR}/Mandelbrot.cpp ${SOURCE_DIR}/GradientGenerator.cpp)
set(HEADERS ${HEADERS_DIR}/Complex.hpp ${HEADERS_DIR}/CompareDoubles.hpp ${HEADERS_DIR}/BitmapFileHeader.hpp
            ${HEADERS_DIR}/BitmapInfoHeader.hpp ${HEADERS_DIR}/Bitmap.hpp ${HEADERS_DIR}/Mandelbrot.hpp
            ${HEADERS_DIR}/Scale.hpp ${HEADERS_DIR}/spline.hpp ${HEADERS_DIR}/RGB.hpp ${HEADERS_DIR}/GradientGenerator.hpp)

add_executable(main ${SOURCE} ${HEADERS})
target_link_directories(main PRIVATE ${GTKMM_LIBRARY_DIRS})
target_include_directories(main PRIVATE ${HEADERS_DIR} ${GTKMM_INCLUDE_DIRS})
target_link_libraries(main ${GTKMM_LIBRARIES})
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}//bin")