/**
 * @file search.hpp
 * @brief 
 * 
 */

#include "pch.hpp"

struct CaseInsensitiveHash
{
    std::size_t operator()(const std::string_view& key) const
    {
        std::size_t hash = 0;
        for (char ch : key)
        {
            hash ^= (std::tolower(ch) + 0x9e3779b9 + (hash << 6) + (hash >> 2));
        }
        return hash;
    }
};

struct CaseInsensitiveEqual
{
    bool operator()(const std::string_view& lhs, const std::string_view& rhs) const
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                          [](char a, char b) {
                              return std::tolower(a) == std::tolower(b);
                          });
    }
};