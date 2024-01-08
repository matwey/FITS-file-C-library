/**
 * @file ifits.hpp
 * @brief
 *
 */

#pragma once

#include "pch.hpp"
#include "hdu.hpp"
#include "details/search.hpp"

class ifits
{
public:
    using header_container_t = std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual>;

    ifits() = default;

    explicit ifits(const boost::asio::io_context &io_context, const std::filesystem::path &filename)
        : file_(io_context, filename, boost::asio::random_access_file::read_only)
    {
        std::ifstream file(filename, std::ios::binary);
        std::string line;

        while (std::getline(file, line) && line.find("END") == std::string::npos)
        {
            std::string key = line.substr(0, 8);
            std::string value = line.substr(8, 30);

            headers_.emplace(key, value);
        }

        std::uint64_t next_hdu_offset = 0;

        while (file_.size() < next_hdu_offset)
        {
            hdus_.push_back(extract_next_HDU(next_hdu_offset));
            next_hdu_offset += hdus_.back().calculate_next_HDU_offset();
        }
    }

private:
    hdu extract_next_HDU(std::uint64_t offset)
    {
        header_container_t headers;
        char data[kSizeHeaderBlock];
        for (;; offset += kSizeHeaderBlock)
        {
            std::size_t n = from_file.read_some_at(offset, boost::asio::buffer(data), error);
            if (error)
            {
                throw std::runtime_error("Error reading file");
            }
            if (n < kSizeHeaderBlock)
            {
                throw std::runtime_error("Invalid file format");
            }

            if (headers.back().type == END)
            {
                break;
            }
        }
        return hdu{headers};
    }

public:
    ifits(const ifits &) = delete;
    ifits(ifits &&) = delete;

    ifits &operator=(const ifits &) = delete;
    ifits &operator=(ifits &) = delete;

    std::list<hdu>::const_iterator cbegin() const
    {
        return hdus_.cbegin();
    }

    std::list<hdu>::const_iterator begin() const
    {
        return hdus_.begin();
    }

    std::list<hdu>::const_iterator cend() const
    {
        return hdus_.cend();
    }

    std::list<hdu>::const_iterator end() const
    {
        return hdus_.end();
    }

    class hdu
    {
    public:
        static constexpr auto kSizeHeaderBlock = 2880;

        hdu(const header_container_t &headers)
            : headers_(headers)
        {
        }

        std::size_t calculate_next_HDU_offset() const
        {
            // Рассчитывает смещение следующего HDU
            const std::size_t data_block_size = (((get_NAXIS_product() * std::abs(get_BITPIX()) / 8) + kSizeHeaderBlock - 1) / kSizeHeaderBlock) * kSizeHeaderBlock;
            const std::size_t offset = get_header_block_count() + data_block_size;

            return offset;
        }

    private:
        std::size_t get_NAXIS_product() const
        {
            auto it = headers_.find("NAXIS");
            if (it == headers_.end())
            {
                throw std::out_of_range("NAXIS not found");
            }

            int product = 1;
            int NAXIS = std::stoi(it->second);
            for (int i = 1; i <= NAXIS; i++)
            {
                std::string key = "NAXIS" + std::to_string(i);
                auto it = headers_.find(key);
                if (it == headers_.end())
                {
                    throw std::out_of_range(key + " not found");
                }
                product *= std::stoi(it->second);
            }

            return product;
        }

        int get_BITPIX() const
        {
            auto it = headers_.find("BITPIX");
            if (it == headers_.end())
            {
                throw std::out_of_range("BITPIX not found");
            }
            return std::stoi(it->second);
        }

        std::size_t get_header_block_count() const
        {
            return (headers_.size() + kSizeHeaderBlock - 1) / kSizeHeaderBlock;
        }

        header_container_t::const_iterator cbegin() const
        {
            return headers_.cbegin();
        }

        header_container_t::const_iterator begin() const
        {
            return headers_.begin();
        }

        header_container_t::const_iterator cend() const
        {
            return headers_.cend();
        }

        header_container_t::const_iterator end() const
        {
            return headers_.end();
        }

        template <class T>
        T value_as(std::string_view key) const
        {
            auto it = headers_.find(key);
            if (it == headers_.end())
            {
                throw std::out_of_range("Not found");
            }

            return *it;
        }

        template <class T>
        std::optional<T> value_as_optional(std::string_view key) const
        {
            auto it = headers_.find(key);
            if (it != headers_.end())
            {
                return it->second;
            }
            return std::nullopt;
        }

        /* паттерн visitor */
        template <class F>
        auto apply(F f) const
        {
            return f(*this);
        }

    private:
        header_container_t headers_;
    };

    template <class T>
    class image_hdu
    {
        auto async_read(T *buffer);
    };

    template <class T>
    auto image_hdu<T>::async_read(T *buffer)
    {
        boost::asio::mutable_buffer buf(buffer, sizeof(T));

        return boost::asio::async_read(file_, buf, boost::asio::transfer_exactly(sizeof(T)),
                                       [](const boost::system::error_code &error, std::size_t bytes_transferred) -> std::size_t
                                       {
                                           if (error)
                                           {
                                               throw std::runtime_error("Error reading file: " + error.message());
                                           }
                                           return bytes_transferred;
                                       });
    }

private:
    boost::asio::random_access_file file_;
    std::list<hdu> hdus_;
};