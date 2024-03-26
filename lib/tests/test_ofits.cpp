#include <gtest/gtest.h>
#include "ofits.hpp"

#define DATA_ROOT PROJECT_ROOT "/tests/data"

TEST(ofits_test, check_constructor)
{
    boost::asio::io_context io_context;

    ofits<std::uint8_t, float> double_hdu_file{io_context, DATA_ROOT "/file.fits", {{200, 300}, {100, 50, 50}}};

    // ofits<std::uint8_t> single_hdu_file(io_context, "single_hdu.fits", {{200, 300}});

    // ofits<std::uint8_t, float> double_hdu_file(io_context, "double_hdu.fits", {{200, 300}, {100, 50, 50}});
}
