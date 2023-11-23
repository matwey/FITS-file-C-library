/**
 * @file ofits.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "pch.hpp"

template<class Args...>
class ofits
{
    ofits(const std::filesystem::path& filename, std::initilizer_list<...> schema);

    class hdu
    {
        template<class T>
        void value_as(string_view key, T value) const;

        /* проитерировать по всем заголовкам */
        const_iterator cbegin() const;
        const_iterator begin() const;
        const_iterator cend() const;
        const_iterator end() const;

        void async_write(U*);  // U это тот самый тип, который передан в Args в нужной позиции
    };
};

/*
	ofits<std::uint8_t> single_hdu_file{"file.fits", {{200, 300}}}; размер 200 x 300
	ofits<std::uint8_t, float> double_hdu_file{"file.fits", {{200, 300}, {100, 50, 50}}};

	std::get<0>(single_hdu_file).value_as("DATE-OBS", "1970-01-01");
	std::get<0>(single_hdu_file).async_write(...);
*/
