project(tests)

add_executable(tests main.cpp test_ifits.cpp test_ofits.cpp)

set_property(TARGET tests PROPERTY CXX_STANDARD 20)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(tests PUBLIC lib_fits gtest gtest_main)

target_link_libraries(tests PRIVATE uring)

target_compile_definitions(tests PRIVATE BOOST_ASIO_HAS_IO_URING)

gtest_discover_tests(tests)
