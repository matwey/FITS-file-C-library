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

    explicit ifits(const std::filesystem::path& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        std::string line;

        // работа с хэдером
        while (getline(file, line))
        {
            if (line.find("END") != std::string::npos)
            {
                break;
            }
            std::string key   = line.substr(0, 8);
            std::string value = line.substr(8, 30);

            headers_.emplace(key, value);
        }

        // потом надо как-то остальные части прочитать (конкретно IMAGE HDU и HDU), наверное, они с каких-то ключевых слов начинаются
        /* 
            hdu tmp()
            hdus_.push_back(hdu) 
        */
    }
    ifits(const ifits&) = delete;
    ifits(ifits&&)      = delete;

    ifits& operator=(const ifits&) = delete;
    ifits& operator=(ifits&)       = delete;

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

    template<class T>
    class image_hdu
    {
        auto async_read(T* buffer);
    };

protected:
    header_container_t headers_;

private:
    std::list<hdu> hdus_;
};