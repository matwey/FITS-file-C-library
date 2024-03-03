/**
 * @file ifits.hpp
 * @brief
 *
 */

#pragma once

#include "pch.hpp"
#include "details/search.hpp"

#if !defined(BOOST_ASIO_HAS_FILE)
#error "BOOST_ASIO_HAS_FILE not defined"
#endif

class ifits
{
public:
    using header_container_t = std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual>;

    header_container_t headers_;

    ifits() = default;

    class hdu
    {
    private:
        ifits &parent_ifits_;
        header_container_t headers_;

    public:
        static constexpr auto kSizeHeaderBlock = 2880;

        hdu(ifits &parent_ifits, const header_container_t &headers)
            : parent_ifits_(parent_ifits),
              headers_(headers)
        {
        }

        hdu(ifits &parent_ifits) : parent_ifits_(parent_ifits) {}

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
            auto it = headers_.find(std::string(key));
            if (it == headers_.end())
            {
                throw std::out_of_range("Not found");
            }

            std::istringstream iss(it->second);
            T value;
            if (!(iss >> value))
            {
                throw std::runtime_error("Failed to convert value");
            }

            return value;
        }

        template <class T>
        std::optional<T> value_as_optional(std::string_view key) const
        {
            auto it = headers_.find(std::string(key));
            if (it != headers_.end())
            {
                std::istringstream iss(it->second);
                T value;
                if (!(iss >> value))
                {
                    throw std::runtime_error("Failed to convert value");
                }
                return value;
            }
            return std::nullopt;
        }

        /* паттерн visitor */
        template <typename Functor>
        auto apply(Functor f)
        {
            if (get_BITPIX() == 8)
            {
                return f(image_hdu<std::uint8_t>(*this));
            }
            else if (get_BITPIX() == 16)
            {
                return f(image_hdu<std::int16_t>(*this));
            }
            else if (get_BITPIX() == 32)
            {
                return f(image_hdu<std::int32_t>(*this));
            }
            else if (get_BITPIX() == 64)
            {
                return f(image_hdu<std::int64_t>(*this));
            }
            else
            {
                throw std::runtime_error("Unsupported BITPIX value");
            }
        }

    public:
        hdu extract_next_HDU(std::uint64_t offset)
        {
            header_container_t headers;
            char data[kSizeHeaderBlock];
            for (;; offset += kSizeHeaderBlock)
            {
                boost::system::error_code error;
                std::size_t n = parent_ifits_.file_.read_some_at(offset, boost::asio::buffer(data, kSizeHeaderBlock), error);
                if (error)
                {
                    throw std::runtime_error("Error reading file");
                }
                if (n < kSizeHeaderBlock)
                {
                    throw std::runtime_error("Invalid file format");
                }

                if (headers.find("END") != headers.end())
                {
                    break;
                }
            }
            return hdu(parent_ifits_);
        }
    };

    explicit ifits(boost::asio::io_context &io_context, const std::filesystem::path &filename)
        : file_(io_context, filename, boost::asio::random_access_file::read_only)
    {
        char buffer[81];
        std::uint64_t next_hdu_offset = 0;

        while (file_.read_some_at(next_hdu_offset, boost::asio::buffer(buffer, 80)) == 80)
        {
            buffer[80] = '\0';

            std::string key = std::string(buffer, 8);
            std::string value = std::string(buffer + 8, 30);
            headers_.emplace(key, value);

            next_hdu_offset += 80;
        }

        while (file_.size() < next_hdu_offset)
        {
            auto new_hdu = hdu(*this).extract_next_HDU(next_hdu_offset);
            hdus_.push_back(new_hdu);
            next_hdu_offset += new_hdu.calculate_next_HDU_offset();
        }
    }

    ifits(const ifits &) = delete;
    ifits(ifits &&) = delete;

    ifits &operator=(const ifits &) = delete;
    ifits &operator=(ifits &) = delete;

public:
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

    template <class T>
    class image_hdu
    {
    public:
        image_hdu(ifits &parent_ifits) : parent_ifits_(parent_ifits) {}

        auto async_read(T *buffer, std::size_t x_coord,
                        std::size_t y_coord,
                        std::size_t z_coord,
                        std::function<void(std::size_t)> callback)
        {
            boost::asio::mutable_buffer buf(buffer, sizeof(T));

            return boost::asio::async_read(parent_ifits_.file_, buf, boost::asio::transfer_exactly(sizeof(T)),
                                           [callback](const boost::system::error_code &error, std::size_t bytes_transferred)
                                           {
                                               if (error)
                                               {
                                                   throw std::runtime_error("Error reading file: " + error.message());
                                               }
                                               callback(bytes_transferred);
                                           });
        }

    private:
        ifits &parent_ifits_;
    };

private:
    boost::asio::random_access_file file_;
    std::list<hdu> hdus_;
};