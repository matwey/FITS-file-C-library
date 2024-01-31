add_executable(test_executable test.cpp)

target_link_libraries(test_executable ifits)

target_include_directories(test_executable PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_property(TARGET test_executable PROPERTY CXX_STANDARD 17)
set_property(TARGET test_executable PROPERTY CXX_STANDARD_REQUIRED ON)
