/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include "UTF8.hpp"
#include <iostream>

// bits	range_first	range_last	bytes	byte0		byte1		byte2		byte3		byte4		byte5
// 7	U+0000		U+007F		1	0xxxxxxx
// 11	U+0080		U+07FF		2	110xxxxx	10xxxxxx
// 16	U+0800		U+FFFF		3	1110xxxx	10xxxxxx	10xxxxxx
// 21	U+10000		U+1FFFFF	4	11110xxx	10xxxxxx	10xxxxxx	10xxxxxx
// 26	U+200000	U+3FFFFFF	5	111110xx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
// 31	U+4000000	U+7FFFFFFF	6	1111110x	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx

#define b10000000 0x80
#define b11000000 0xc0
#define b11100000 0xe0
#define b11110000 0xf0
#define b11111000 0xf8
#define b11111100 0xfc
#define b11111110 0xfe
#define b00111111 0x3f
#define b00011111 0x1f
#define b00001111 0x0f
#define b00000111 0x07
#define b00000011 0x03
#define b00000001 0x01

uint32_t get_unicode_char(std::string::const_iterator &si) {
	uint32_t unicode = 0;

	if        ((*si & b10000000) ==     0    ) { // 1-byte character
		unicode = *(si++);
	} else if ((*si & b11100000) == b11000000) { // 2-byte character
		unicode = *(si++) & b00011111;
		unicode = (unicode << 6) | (*(si++) & b00111111);
	} else if ((*si & b11110000) == b11100000) { // 3-byte character
		unicode = *(si++) & b00001111;
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
	} else if ((*si & b11111000) == b11110000) { // 4-byte character
		unicode = *(si++) & b00000111;
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
	} else if ((*si & b11111100) == b11111000) { // 5-byte character
		unicode = *(si++) & b00000011;
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
	} else if ((*si & b11111110) == b11111100) { // 6-byte character
		unicode = *(si++) & b00000001;
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
		unicode = (unicode << 6) | (*(si++) & b00111111);
	}

	return unicode;
}

void remove_last_char(std::string &s) {
	while (!s.empty() && ((s[s.size() - 1] & b11000000) == b10000000)) {
		s.resize(s.size() - 1);
	}
	if (!s.empty()) {
		s.resize(s.size() - 1);
	}
}

void append_unicode_char(std::string &s, uint32_t character) {
	if (character < 0x80) {
		s.push_back((char)(character));
	} else if (character < 0x800) {
		s.push_back((char)(b11000000 | (character >> 6)));
		s.push_back((char)(b10000000 | (character & b00111111)));
	} else if (character < 0x10000) {
		s.push_back((char)(b11100000 | (character >> 12)));
		s.push_back((char)(b10000000 | ((character >> 6) & b00111111)));
		s.push_back((char)(b10000000 | (character & b00111111)));
	} else if (character < 0x200000) {
		s.push_back((char)(b11110000 | (character >> 18)));
		s.push_back((char)(b10000000 | ((character >> 12) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 6) & b00111111)));
		s.push_back((char)(b10000000 | (character & b00111111)));
	} else if (character < 0x4000000) {
		s.push_back((char)(b11111000 | (character >> 24)));
		s.push_back((char)(b10000000 | ((character >> 18) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 12) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 6) & b00111111)));
		s.push_back((char)(b10000000 | (character & b00111111)));
	} else {
		s.push_back((char)(b11111100 | (character >> 30)));
		s.push_back((char)(b10000000 | ((character >> 24) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 18) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 12) & b00111111)));
		s.push_back((char)(b10000000 | ((character >> 6) & b00111111)));
		s.push_back((char)(b10000000 | (character & b00111111)));
	}
}

//uint32_t get_unicode_char(std::string::iterator &si) {
//	uint32_t unicode = 0;
//
//	if        ((*si & b10000000) ==     0    ) { // 1-byte character
//		unicode = *(si++);
//	} else if ((*si & b11100000) == b11000000) { // 2-byte character
//		unicode = *(si++) & b00011111;
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//	} else if ((*si & b11110000) == b11100000) { // 3-byte character
//		unicode = *(si++) & b00001111;
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//	} else if ((*si & b11111000) == b11110000) { // 4-byte character
//		unicode = *(si++) & b00000111;
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//	} else if ((*si & b11111100) == b11111000) { // 5-byte character
//		unicode = *(si++) & b00000011;
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//	} else if ((*si & b11111110) == b11111100) { // 6-byte character
//		unicode = *(si++) & b00000001;
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//		unicode = (unicode << 6) | (*(si++) & b00111111);
//	}
//
//	return unicode;
//}

#if 0
#include <iostream>
int main(int argc, char **argv) {
	std::string a(argv[1]);
	for (std::string::iterator i = a.begin(); i != a.end();) {
		uint32_t x = get_unicode_char(i);
		std::cout << x << std::endl;
	}
}
#endif
