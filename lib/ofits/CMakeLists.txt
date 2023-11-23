set(LIB_OFITS_INCLUDE_ROOT                      ${LIB_OFITS_ROOT}/include)

set(LIB_OFITS_DETAILS_ROOT                      ${LIB_OFITS_INCLUDE_ROOT}/details)

set(LIB_OFITS_INCLUDE_DIRECTORIES               ${LIB_OFITS_INCLUDE_ROOT})

set(LIB_OFITS_HEADER_FILES                      
                                                ${LIB_OFITS_INCLUDE_ROOT}/ofits.hpp
                                                ${LIB_OFITS_INCLUDE_ROOT}/pch.hpp
                                                ${LIB_OFITS_DETAILS_ROOT}/search.hpp)

add_library(ofits.impl SHARED                   ${LIB_OFITS_HEADER_FILES})  

target_include_directories(ofits.impl PRIVATE   ${LIB_OFITS_INCLUDE_DIRECTORIES})