cmake_minimum_required(VERSION 3.1)

project (internetbot)

set (INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin CACHE STRING "Instalation path")

add_definitions(-std=c++1y -ggdb -D_GLIBCXX_DEBUG)

add_subdirectory(src)


