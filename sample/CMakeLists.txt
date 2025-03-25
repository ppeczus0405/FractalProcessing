cmake_minimum_required(VERSION 3.21)

if (DEFINED QT6_INSTALL_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${QT6_INSTALL_DIR}")

    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    # Run a Python one-liner to find all lib/cmake folders
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E env PYTHONIOENCODING=utf-8
                ${Python3_EXECUTABLE} -c
                "import glob; import os; print(';'.join(glob.glob(os.path.join(r'${QT6_INSTALL_DIR}', '**/lib/cmake'), recursive=True)))"
        OUTPUT_VARIABLE QT6_CMAKE_PATHS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  foreach(_qt_cmake_path ${QT6_CMAKE_PATHS})
      if (IS_DIRECTORY "${_qt_cmake_path}")
          list(APPEND CMAKE_PREFIX_PATH "${_qt_cmake_path}")
      endif()
  endforeach()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Prioritize 6.8.2 <= version < 7 due to tested compatibility. 
find_package(Qt6 6.8.2 COMPONENTS Widgets Gui QUIET)

if (NOT Qt6_FOUND)
  find_package(Qt6 COMPONENTS Widgets Gui QUIET)
endif()

if (NOT Qt6_FOUND)
    message(FATAL_ERROR
        "Could not find Qt6.\n"
        "Set QT6_INSTALL_DIR to your Qt6 installation path.\n"
        "Current QT6_INSTALL_DIR: '${QT6_INSTALL_DIR}'\n"
    )
endif()

message(STATUS "Qt6 version found: ${Qt6_VERSION}")

if (Qt6_VERSION VERSION_LESS "6.8.2")
    message(WARNING
        "Qt6 version is too old: ${Qt6_VERSION}\n"
        "Recommended version is 6.8.2 or higher.\n"
        "You may encounter compatibility issues."
    )
endif()

file(GLOB_RECURSE QT_SAMPLE_APP_SOURCES "src/*.cpp" "include/*.hpp")
add_executable(ImagePlaneWithPanel ${QT_SAMPLE_APP_SOURCES})
target_include_directories(ImagePlaneWithPanel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ImagePlaneWithPanel PRIVATE Qt6::Widgets)
