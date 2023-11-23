cmake_minimum_required(VERSION 3.5.0)
project(
    lib_fits 
    LANGUAGES C CXX 
    VERSION 1.0.0)

    
set(CMAKE_CXX_STANDARD                                   17)
set(CMAKE_CXX_STANDARD_REQUIRED                          True)


set(LIB_ROOT		                                         ${CMAKE_CURRENT_LIST_DIR})
set(LIB_IFITS_ROOT                                       ${LIB_ROOT}/ifits)
set(LIB_OFITS_ROOT                                       ${LIB_ROOT}/ofits)
set(LIB_IFITS_INCLUDE_DIR                                ${LIB_IFITS_ROOT}/include)
set(LIB_OFITS_INCLUDE_DIR                                ${LIB_OFITS_ROOT}/include)


add_compile_definitions(UNICODE)

add_subdirectory(ifits)
add_subdirectory(ofits)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ifits ofits)