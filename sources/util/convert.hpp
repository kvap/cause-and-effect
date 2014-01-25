/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <stdint.h>
#ifdef __unix__
#include <endian.h>
#endif

class BadConversion : public std::runtime_error {
	public:
		BadConversion(const std::string &s) : std::runtime_error(s) {
		}
};

// in-place conversion from std::string to any type
template<typename T>
inline void convert(const std::string &s, T &x, bool forbid_remainder = true) {
	std::istringstream i(s);
	char c;
	if (!(i >> x) || (forbid_remainder && i.get(c))) {
		throw BadConversion(s);
	}
}

// conversion from std::string to any type
template<typename T>
inline T convertTo(const std::string &s, bool forbid_remainder = true) {
	T x;
	convert(s, x, forbid_remainder);
	return x;
}

// conversion from any type to std::string
template<typename T>
inline std::string stringify(const T &x) {
	std::ostringstream o;
	if (!(o << x)) {
		throw BadConversion(std::string("stringify(") + typeid(x).name() + ")");
	}
	return o.str();
}

// big-endian float -> native float
inline float be2float(float number) {
	uint32_t *intform = (uint32_t*)&number;
#ifdef __unix__
	*intform = be32toh(*intform);
#else
	*intform = ((*intform>>24)&0xff) | // move byte 3 to byte 0
                    ((*intform<<8)&0xff0000) | // move byte 1 to byte 2
                    ((*intform>>8)&0xff00) | // move byte 2 to byte 1
                    ((*intform<<24)&0xff000000); // byte 0 to byte 3
#endif
	return number;
}

// delete leading and trailing spaces from a string
inline std::string trim(const std::string& str) {
	int first = str.find_first_not_of(" \t\n");
	int last = str.find_last_not_of(" \t\n");
	return str.substr(first, last - first + 1);
}

#endif
