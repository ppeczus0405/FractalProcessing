cmake_minimum_required(VERSION 3.21)
project(FractalGenerator)
set(CMAKE_CXX_STANDARD 17)

INCLUDE(FindPkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(FractalGenerator src/main.cpp)

target_link_libraries(FractalGenerator ${GTKMM_LIBRARIES})