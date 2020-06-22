cmake_minimum_required(VERSION 3.15)
project(MPI LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

link_libraries(${JSONCPP_LIBRARIES})
include_directories("${JSONCPP_INCLUDE_DIRS}")

find_package(MPI REQUIRED)

add_executable(MPI main.cpp headers/config_processing.h headers/matrix_manipulations.h headers/matrix.h)
target_link_libraries(MPI MPI::MPI_CXX)
target_link_libraries(MPI ${JSONCPP_LIBRARIES})