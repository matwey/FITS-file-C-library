#include <gtest/gtest.h>
#include "ifits.hpp"

#define DATA_ROOT PROJECT_ROOT "/tests/data"

TEST(ifitsTest, PrintHeaders)
{
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/movie-64.fits";

    ifits movie64_fits(io_context, filename);

    std::cout << "Movie-64 fits file" << '\n';

    std::cout << "Headers:\n";
    for (const auto &hdu : movie64_fits.get_hdus())
    {
        std::cout << "---New HDU---" << '\n';
        for (const auto &[key, value] : hdu.get_headers())
        {
            std::cout << key << ": " << value << '\n';
        }
    }

    filename = DATA_ROOT "/gradient.fits";

    ifits gradient_fits(io_context, filename);

    std::cout << "Gradient fits file" << '\n';

    std::cout << "Headers:\n";
    for (const auto &hdu : gradient_fits.get_hdus())
    {
        std::cout << "---New HDU---" << '\n';
        for (const auto &[key, value] : hdu.get_headers())
        {
            std::cout << key << ": " << value << '\n';
        }
    }
}

TEST(ifitsTest, CheckNotExistingHeader)
{
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/movie-64.fits";

    ifits movie64_fits(io_context, filename);

    try
    {
        std::string value = movie64_fits.get_hdus().front().value_as<std::string>("NON_EXISTING_KEY");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_STREQ(e.what(), "Not found");
    }

    filename = DATA_ROOT "/gradient.fits";

    ifits gradient_fits(io_context, filename);

    try
    {
        std::string value = gradient_fits.get_hdus().front().value_as<std::string>("NON_EXISTING_KEY");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_STREQ(e.what(), "Not found");
    }
}

TEST(ifitsTest, CheckValues)
{
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/movie-64.fits";

    ifits movie64_fits(io_context, filename);

    for (const auto &hdu : movie64_fits.get_hdus())
    {
        for (const auto &[key, value] : hdu.get_headers())
        {
            EXPECT_EQ(value, hdu.value_as<std::string>(key));
        }
    }

    filename = DATA_ROOT "/gradient.fits";

    ifits gradient_fits(io_context, filename);

    for (const auto &hdu : gradient_fits.get_hdus())
    {
        for (const auto &[key, value] : hdu.get_headers())
        {
            EXPECT_EQ(value, hdu.value_as<std::string>(key));
        }
    }
}

TEST(ifitsTest, CheckValueAsOptional)
{
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/movie-64.fits";

    ifits movie64_fits(io_context, filename);

    for (const auto &hdu : movie64_fits.get_hdus())
    {
        for (const auto &[key, value] : hdu.get_headers())
        {
            EXPECT_EQ(value, hdu.value_as_optional<std::string>(key));
        }
    }
}

TEST(ifitsTest, CheckNotExistingHeaderOptional)
{
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/movie-64.fits";

    ifits movie64_fits(io_context, filename);

    std::optional<std::string> value = movie64_fits.get_hdus().front().value_as_optional<std::string>("NON_EXISTING_KEY");
    EXPECT_EQ(value, std::nullopt);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
