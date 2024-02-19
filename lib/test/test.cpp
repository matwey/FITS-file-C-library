#include <boost/asio/io_context.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ifits.hpp>

void test_reading_headers()
{
    boost::asio::io_context io_context;
    std::filesystem::path filename = "test_file.txt";

    ifits obj(io_context, filename);

    assert(obj.headers_.size() == 2);
    auto it = obj.headers_.find("header1");
    assert(it != obj.headers_.end() && it->second == "value1");
    auto it2 = obj.headers_.find("header2");
    assert(it2 != obj.headers_.end() && it2->second == "value2");
}

void test_extracting_hdus()
{
    boost::asio::io_context io_context;
    std::filesystem::path filename = "test_file.txt";

    ifits obj(io_context, filename);

    // assert(obj.hdus_.size() == 2);
}

int main()
{
    test_reading_headers();
    test_extracting_hdus();

    return 0;
}