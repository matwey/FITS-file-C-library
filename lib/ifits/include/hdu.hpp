/**
 * @file hdu.hpp
 * @brief 
 * 
 */

#pragma once

#include "ifits.hpp"

class hdu : public ifits
{
    static constexpr auto kSizeHeaderBlock = 2880;

private:
    hdu(const ifits& fits)
        : fits_(fits)
    { }

public:
    std::size_t calculate_next_HDU_offset() const
    {
        const std::size_t data_block_size = (((get_NAXIS_product() * std::abs(get_BITPIX()) / 8) + kSizeHeaderBlock - 1) / kSizeHeaderBlock) * kSizeHeaderBlock;
        const std::size_t offset          = get_header_block_count() + data_block_size;

        return offset;
    }

private:
    std::size_t get_NAXIS_product() const
    { }

    int get_BITPIX() const
    { }

    std::size_t get_header_block_count() const
    { }

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

    template<class T>
    T value_as(std::string_view key) const
    {
        auto it = headers_.find(key);
        if (it == headers_.end())
        {
            throw std::out_of_range("Not found");
        }

        return *it;
    }
    template<class T>
    std::optional<T> value_as_optional(std::string_view key) const
    {
        try
        {
            auto it = headers_.find(key);
            if (it != headers_.end())
            {
                return headers_[key];
            }
            else
            {
                return std::nullopt;
            }
        }
        catch (const std::exception& e)
        {
            return 1;
        }
    }

    /* паттерн visitor */
    template<class F>
    auto apply(F f) const
    {
        return f(*this);
    }

private:
    const ifits& fits_;
};