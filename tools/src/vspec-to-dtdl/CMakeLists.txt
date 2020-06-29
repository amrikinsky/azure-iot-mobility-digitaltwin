cmake_minimum_required(VERSION 3.10)

project(vspec-to-dtdl)

add_compile_options(-std=c++11)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS program_options)

include_directories(
    ../../third-party/jsoncpp/src
    ../../third-party/yanml-cpp/src/yaml-cpp/include
    ../vspec-definition
    ../dtdl-definition
    ../../third-party/boost/include)

link_directories(
    ../vspec-definition
    ../dtdl-definition
    ../../third-party/yanml-cpp/src/yaml-cpp-build
    ../../third-party/jsoncpp/src/jsoncpp-build/lib
    ../../third-party/boost/lib)

file(GLOB SOURCES "*.cpp")

add_executable(vspec-to-dtdl ${SOURCES})

target_link_libraries(vspec-to-dtdl vspec-definition dtdl-definition yaml-cpp jsoncpp boost_program_options ${Boost_LIBRARIES})
