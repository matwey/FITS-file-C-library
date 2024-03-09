cmake_minimum_required(VERSION 3.5.0)
project(
    lib_fits 
    LANGUAGES CXX 
    VERSION 1.0.0)

set(LIB_IFITS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ifits)
set(LIB_OFITS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ofits)   

set(IFITS_INCLUDE_DIR ${LIB_IFITS_DIR}/include)
include_directories(${IFITS_INCLUDE_DIR})

set(OFITS_INCLUDE_DIR ${LIB_OFITS_DIR}/include)
include_directories(${OFITS_INCLUDE_DIR})

set(LIB_FITS_HEADER_FILES ${IFITS_INCLUDE_DIR}/ifits.hpp
                          ${IFITS_INCLUDE_DIR}/details/search.hpp
                          ${OFITS_INCLUDE_DIR}/ofits.hpp)

add_library(lib_fits INTERFACE ${LIB_IFITS_HEADER_FILES})      

target_compile_definitions(lib_fits INTERFACE PROJECT_ROOT="${PROJECT_SOURCE_DIR}")

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.84.0)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(lib_fits ${Boost_LIBRARIES})

add_subdirectory(tests)
add_subdirectory(googletest)