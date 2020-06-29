cmake_minimum_required(VERSION 3.10)

project(third-party VERSION 1.0)

include(ExternalProject)


# Vehicle Signal Specification

set(VSS_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/vehicle_signal_specification)

ExternalProject_Add (vehicle_signal_specification
    GIT_REPOSITORY https://github.com/GENIVI/vehicle_signal_specification
    GIT_TAG master
    PREFIX ${VSS_INSTALL_LOCATION}
    # Disable all other steps
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)


# BOOST program_options

set( boost_URL "http://sourceforge.net/projects/boost/files/boost/1.73.0/boost_1_73_0.tar.bz2" )
set( boost_SHA1 "6d6ed02b29c860fd21b274fc4e1f820855e765e9" )
set( boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/boost )
set( boost_INCLUDE_DIR ${boost_INSTALL}/include )
set( boost_LIB_DIR ${boost_INSTALL}/lib )

ExternalProject_Add( external_boost
        PREFIX boost
        URL ${boost_URL}
        URL_HASH SHA1=${boost_SHA1}
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=program_options --prefix=${boost_INSTALL}
        BUILD_COMMAND ./b2 install link=static variant=release threading=multi runtime-link=static
        INSTALL_COMMAND ""
        INSTALL_DIR ${boost_INSTALL} )

set( boost_LIBRARY_SUFFIX .a )

add_library( boost::program_options STATIC IMPORTED )

set_property( TARGET boost::program_options PROPERTY IMPORTED_LOCATION ${boost_LIB_DIR}/libboost_program_options${boost_LIBRARY_SUFFIX} )
set_property( TARGET boost::program_options PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${boost_INCLUDE_DIR} )
add_dependencies( boost::program_options external_boost )

# yaml-cpp

set(YAML_CPP_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/yanml-cpp)

ExternalProject_Add (yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG master
    PREFIX ${YAML_CPP_INSTALL_LOCATION}
    # Disable all other steps
    INSTALL_COMMAND ""
)


# jsoncpp

set(JSONCPP_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/jsoncpp)

ExternalProject_Add (jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp
    GIT_TAG master
    PREFIX ${JSONCPP_INSTALL_LOCATION}
    # Disable all other steps
    INSTALL_COMMAND ""
)
