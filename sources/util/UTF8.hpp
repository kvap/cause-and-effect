/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef UTF8_HPP
#define UTF8_HPP

#include <stdint.h>
#include <string>

//uint32_t get_unicode_char(std::string::iterator &si);
uint32_t get_unicode_char(std::string::const_iterator &si);
void remove_last_char(std::string &s);
void append_unicode_char(std::string &s, uint32_t character);

#endif

