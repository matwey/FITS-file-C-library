set(LIB_IFITS_HEADER_FILES ${IFITS_INCLUDE_DIR}/ifits.h
                           ${IFITS_INCLUDE_DIR}/details/search.hpp)

add_library(ifits ${LIB_IFITS_HEADER_FILES})

target_precompile_headers(ifits PRIVATE ${IFITS_INCLUDE_DIR}/pch.hpp)

find_package(Boost REQUIRED COMPONENTS asio)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(ifits ${Boost_LIBRARIES})