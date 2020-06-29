cmake_minimum_required(VERSION 3.10)

project(dtdl-definition)

add_compile_options(-std=c++11)

include_directories(../../third-party/jsoncpp/src)

file(GLOB SOURCES "*.cpp")

add_library(dtdl-definition STATIC ${SOURCES})
