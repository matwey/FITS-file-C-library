// Ifits.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <optional>
#include <sstream>
#include <filesystem>
#include <string_view>
#include <map>

using header = std::unordered_multimap<std::string_view, std::string_view>;

class ifits {
	explicit ifits(const std::filesystem::path& filename);
	ifits(const ifits&) = delete;
	ifits(ifits&&) = delete;

	void operator=(const ifits&) = delete;
	void operator=(ifits&&) = delete;

	const_iterator cbegin() const;
	const_iterator begin() const;
	const_iterator cend() const;
	const_iterator end() const;

	class hdu {
	private:
		hdu(header header_data); // пока непонятно какие еще объекты класса, поэтому непонятно, что передавать в конструктор
	public:
		// метод для вычисления смещения до следующего HDU
		std::size_t calculateNextHDUOffset() const {
			// считать по NAXIS и BITPIX ? 
		}

		const_iterator cbegin() const {
			return header_data_.cbegin();
		}

		const_iterator begin() const {
			return header_data_.begin();
		}

		const_iterator cend() const {
			return header_data_.cend();
		}

		const_iterator end() const {
			return header_data_.end();
		}

		template<class T>
		T value_as(std::string_view key) const {
			try {
				// выглядит как лишнее копирование, но ничего другого пока не пришло
				std::string lowercase(key);
				std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);
				std::string_view lower_key(lowercase.data(), lowercase.size());

				auto it = header_data_.find(lower_key);
				if (it != header_data_.end()) {
					return header_data_[lower_key]; // достаточно ли возвращать люое значение ?
				}
				else {
					throw std::runtime_error("Key not found.");
				}
			}
			catch (const std::exception& e) {
				return 1; // потом нужно написать свой класс для ошибок и просто возвращать e.what() в таком случае
			}
		}
		template<class T>
		std::optional<T> value_as_optional(std::string_view key) const {
			try {
				std::string lowercase(key);
				std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);
				std::string_view lower_key(lowercase.data(), lowercase.size());

				auto it = header_data_.find(lower_key);
				if (it != header_data_.end()) {
					return header_data_[lower_key];
				}
				else {
					return NULL; // вовзращаю нуль, тк опциональный метод
				}
			}
			catch (const std::exception& e) {
				return 1;
			}
		}

		/* паттерн visitor */
		template<class F>
		auto apply(F f) const {
			return f(*this); // это не то, да? насколько я поняла apply нужен для того, 
					// чтобы применять какие-то пользовательские функции к HDU
		}

		/*
			struct functor {
				void operator() (image_hdu<std::uint8_t> im) {

				}
				void operator() (image_hdu<std::int16_t> im) {

				}
				void operator() (image_hdu<std::int32_t> im) {

				}
			};

			hdu.apply(functor{});



			hdu.apply([] (auto x) {
				x.async_read(...);

				x имеет тип image_hdu<T>
			});
		*/

	private:
		// хранение заголовков HDU, нужен быстрый поиск поэтому unordered, multimap,значений может быть несколько
		header header_data_;  
	};

	template<class T>
	class image_hdu {
		auto async_read(T* buffer);
	};
};

template<class Args...>
class ofits {
	ofits(const std::filesystem::path& filename, std::initilizer_list<...> schema);

	class hdu {
		template<class T>
		void value_as(string_view key, T value) const;

		/* проитерировать по всем заголовкам */
		const_iterator cbegin() const;
		const_iterator begin() const;
		const_iterator cend() const;
		const_iterator end() const;

		void async_write(U*); // U это тот самый тип, который передан в Args в нужной позиции
	};
};

/*
	ofits<std::uint8_t> single_hdu_file{"file.fits", {{200, 300}}}; размер 200 x 300
	ofits<std::uint8_t, float> double_hdu_file{"file.fits", {{200, 300}, {100, 50, 50}}};

	std::get<0>(single_hdu_file).value_as("DATE-OBS", "1970-01-01");
	std::get<0>(single_hdu_file).async_write(...);
*/
