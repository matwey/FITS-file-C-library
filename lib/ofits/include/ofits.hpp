#include "pch.hpp"
#include "details/search.hpp"

#if !defined(BOOST_ASIO_HAS_FILE)
#error "BOOST_ASIO_HAS_FILE not defined"
#endif

// так вызываем ofits<std::uint8_t, float> double_hdu_file{"file.fits", {{200, 300}, {100, 50, 50}}};

template <class... Args>
class ofits
{
public:
    ofits() = default;
    ofits(boost::asio::io_context &io_context, const std::filesystem::path &filename, std::initializer_list<std::initializer_list<std::size_t>> schema)
        : file_(io_context, filename, boost::asio::random_access_file::write_only),
          hdus_(std::apply([&](auto &&...args)
                           { return std::make_tuple(hdu<Args>(*this, args)...); },
                           schema))
    {
    }

    template <std::size_t N>
    auto &get_hdu()
    {
        return std::get<N>(hdus_);
    }

    template <typename T>
    class hdu
    {
        hdu() = default;
        hdu(ofits &parent_ofits, const std::initializer_list<std::size_t> &hdu_schema)
            : parent_ofits_(parent_ofits), header_block_(2880, ' '), headers_written_(0)
        {
            write_header("SIMPLE", "T");
            write_header("BITPIX", std::to_string(get_bitpix_for_type()));
            write_header("NAXIS", std::to_string(hdu_schema.size()));

            std::size_t i = 0;
            for (const auto &size : hdu_schema)
            {
                write_header("NAXIS" + std::to_string(++i), std::to_string(size));
            }

            write_header("EXTEND", "T");
            write_header("END", "");
        }

        void write_header(const std::string &key, const std::string &value)
        {
            std::string header = key + " = " + value;
            header.resize(80, ' ');
            size_t position = headers_written_ * 80;
            boost::asio::write_at(parent_ofits_.file_, position, boost::asio::buffer(header));
            ++headers_written_;
        }

        
        // проверить на наличие места и записать
        template <class T>
        void value_as(string_view key, T value) const {
            // запись хэдеров
        }

        static int get_bitpix_for_type()
        {
            if constexpr (std::is_same_v<T, std::uint8_t>)
            {
                return 8;
            }
            else if constexpr (std::is_same_v<T, std::int16_t>)
            {
                return 16;
            }
            else if constexpr (std::is_same_v<T, std::int32_t>)
            {
                return 32;
            }
            else if constexpr (std::is_same_v<T, std::int64_t>)
            {
                return 64;
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                return -32;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return -64;
            }
            else
            {
                static_assert(sizeof(T) == -1, "Unsupported data type");
            }
        }

    private:
        ofits &parent_ofits_;
        std::string header_block_;
        size_t headers_written_;
    };

private:
    boost::asio::random_access_file file_;
    std::tuple<hdu<Args>...> hdus_;
};
