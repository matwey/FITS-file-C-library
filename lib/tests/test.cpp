#include <gtest/gtest.h>
#include "ifits.hpp"

#define DATA_ROOT PROJECT_ROOT "/tests/data"

TEST(ifitsTest, ReadFITSFileAndCheckHeaders) {
    boost::asio::io_context io_context;

    std::filesystem::path filename = DATA_ROOT "/gradient.fits";

    ifits file(io_context, filename);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

