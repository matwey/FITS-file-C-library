#include <gtest/gtest.h>
#include "ofits.hpp"

#define DATA_ROOT PROJECT_ROOT "/tests/data"

TEST(ofits_test, check_single_hdu)
{
    boost::asio::io_context io_context;

    ofits<std::uint8_t> single_hdu_file{io_context, DATA_ROOT "/single_hdu.fits", {{{200, 300}}}};

    auto& hdu_0 = single_hdu_file.get_hdu<0>();

    EXPECT_EQ(hdu_0.get_headers_written(), 6);

    single_hdu_file.value_as<0>("XTENSION", "TABLE ");
    
    EXPECT_EQ(hdu_0.get_headers_written(), 7);


}

TEST(ofits_test, check_double_hdu)
{
    boost::asio::io_context io_context;

    ofits<std::uint8_t, float> double_hdu_file{io_context, DATA_ROOT "/double_hdu.fits", {{{200, 300}, {100, 50, 50}}}};

    auto& hdu_0 = double_hdu_file.get_hdu<0>();

    auto& hdu_1 = double_hdu_file.get_hdu<1>();

    EXPECT_EQ(hdu_0.get_headers_written(), 6);

    EXPECT_EQ(hdu_1.get_headers_written(), 7);

    double_hdu_file.value_as<0>("DATE-OBS", "1970-01-01");

    EXPECT_EQ(hdu_0.get_headers_written(), 7);

    double_hdu_file.value_as<1>("DATE-OBS", "1991-12-26");

    EXPECT_EQ(hdu_1.get_headers_written(), 8);
}