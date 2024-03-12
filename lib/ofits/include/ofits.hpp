/**
 * @file ofits.hpp
 * @brief
 *
 */

#include "pch.hpp"
#include "details/search.hpp"

#if !defined(BOOST_ASIO_HAS_FILE)
#error "BOOST_ASIO_HAS_FILE not defined"
#endif

template <class Args...>
class ofits
{
    ofits(boost::asio::io_context &io_context, const std::filesystem::path &filename, std::initilizer_list<...> schema)
        : file_(io_context, filename, std::move(schema), boost::asio::random_access_file::write_only)
    {
        file_.open(filename, boost::asio::random_access_file::read_write);

        for (const auto &arg : schema)
        {
            //int dimension = arg.first;
            //std::vector<int> axis_sizes = arg.second;

            std::string dimension_header = "NAXIS " + std::to_string(dimension) + "\n";
            //записать

            for (int i = 0; i < dimension; ++i)
            {
                std::string axis_header = "NAXIS" + std::to_string(i + 1) + " " + std::to_string(axis_sizes[i]) + "\n";
                //записать
            }

        }

        std::string end_header = "END\n";
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
            // запись хэдеров
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
