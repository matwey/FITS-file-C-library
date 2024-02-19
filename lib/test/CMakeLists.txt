add_executable(test_executable test.cpp)

#target_link_libraries(test_executable lib_fits)
target_include_directories(test_executable PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_executable uring)
target_compile_definitions(test_executable PRIVATE BOOST_ASIO_HAS_IO_URING)

set_property(TARGET test_executable PROPERTY CXX_STANDARD 17)
set_property(TARGET test_executable PROPERTY CXX_STANDARD_REQUIRED ON)
