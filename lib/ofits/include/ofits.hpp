/**
 * @file ofits.hpp
 * @brief
 *
 */

#include "pch.hpp"
#include <ifits/details/search.hpp>

#if !defined(BOOST_ASIO_HAS_FILE)
#error "BOOST_ASIO_HAS_FILE not defined"
#endif

template <class Args...>
class ofits
{
    ofits(boost::asio::io_context &io_context, const std::filesystem::path &filename, std::initilizer_list<...> schema)
        : file_(io_context, filename, std::move(schema), boost::asio::random_access_file::write_only)
    {
    }

    class hdu
    {
        using header_container_t = std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual>;
        hdu(ofits &parent_ofits)
            : parent_ofits_(parent_ofits)
        {
        }
        template <class T>
        void value_as(string_view key, T value) const {

        }

        /* проитерировать по всем заголовкам */
        header_container_t::const_iterator cbegin() const {
            return headers_.cbegin();
        }

        header_container_t::const_iterator begin() const {
            return headers_.begin();
        }

        header_container_t::const_iterator cend() const {
            return headers_.cend();
        }

        header_container_t::const_iterator end() const {
            return headers_.end();
        }

        void async_write(U *); // U это тот самый тип, который передан в Args в нужной позиции

    private:
        header_container_t headers_;    
    };

private:
    boost::asio::random_access_file file_;
};

/*
    ofits<std::uint8_t> single_hdu_file{"file.fits", {{200, 300}}}; размер 200 x 300
    ofits<std::uint8_t, float> double_hdu_file{"file.fits", {{200, 300}, {100, 50, 50}}};

    std::get<0>(single_hdu_file).value_as("DATE-OBS", "1970-01-01");
    std::get<0>(single_hdu_file).async_write(...);
*/
