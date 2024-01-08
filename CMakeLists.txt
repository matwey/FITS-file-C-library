cmake_minimum_required(VERSION 3.5.0)
project(
    lib_fits 
    LANGUAGES C CXX 
    VERSION 1.0.0)

set(LIB_IFITS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ifits)
set(LIB_OFITS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ofits)   

set(IFITS_INCLUDE_DIR ${LIB_IFITS_DIR}/include)
include_directories(${IFITS_INCLUDE_DIR})

set(OFITS_INCLUDE_DIR ${LIB_OFITS_DIR}/include)
include_directories(${OFITS_INCLUDE_DIR})

add_subdirectory(ifits)
add_subdirectory(ofits)
add_subdirectory(tests)