cmake_minimum_required(VERSION 3.10)

project(vspec-definition)

add_compile_options(-std=c++11)

include_directories(../../third-party/yaml-cpp/src/yaml-cpp/include/yaml-cpp)

file(GLOB SOURCES "*.cpp")

add_library(vspec-definition STATIC ${SOURCES})
