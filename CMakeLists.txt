cmake_minimum_required(VERSION 3.15)
project(MPI LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

link_libraries(${JSONCPP_LIBRARIES})
include_directories("${JSONCPP_INCLUDE_DIRS}")

find_package(MPI REQUIRED)

add_executable(MPI main.cpp headers/config_processing.h headers/matrix_manipulations.h
        headers/matrix.h headers/heat_calculation.h headers/visualisation.h)

target_link_libraries(MPI MPI::MPI_CXX)
target_link_libraries(MPI ${JSONCPP_LIBRARIES})


add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(MPI ${ImageMagick_LIBRARIES})