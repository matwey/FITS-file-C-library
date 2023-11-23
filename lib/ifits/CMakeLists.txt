set(LIB_IFITS_INCLUDE_ROOT                      ${LIB_IFITS_ROOT}/include)

set(LIB_IFITS_DETAILS_ROOT                      ${LIB_IFITS_INCLUDE_ROOT}/details)

set(LIB_IFITS_INCLUDE_DIRECTORIES               ${LIB_IFITS_INCLUDE_ROOT})

set(LIB_IFITS_HEADER_FILES                      ${LIB_IFITS_INCLUDE_ROOT}/hdu.hpp
                                                ${LIB_IFITS_INCLUDE_ROOT}/ifits.hpp
                                                ${LIB_IFITS_INCLUDE_ROOT}/pch.hpp
                                                ${LIB_IFITS_DETAILS_ROOT}/search.hpp)

add_library(ifits.impl SHARED                   ${LIB_IFITS_HEADER_FILES})  

target_include_directories(ifits.impl PRIVATE   ${LIB_IFITS_INCLUDE_DIRECTORIES})