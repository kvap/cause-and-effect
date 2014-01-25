/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef FONTS_HPP
#define FONTS_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font;
class Fonts {
	public:
		static Font *genFont(const std::string filename, size_t size);
};

enum Align {
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};

class Font {
	private:
		struct Glyph {
			float tex_left, tex_top, tex_right, tex_bottom;
			int x, y, advance, width, height, bearingX, bearingY;
			unsigned char *data;
		};
		std::map<uint32_t, Glyph*> glyphs; // glyph_id -> glyph (do not repeat)
		std::map<uint32_t, Glyph*> unicode2glyph; // unicode -> glyph (may repeat)
		//std::map<std::pair<uint32_t, uint32_t>, float> kerning;
		int pixel_height;
	public:
		GLuint texture; // generated font texture
		Font(const std::string filename, size_t size);
		~Font();
		size_t getLineHeight() const;
		size_t getCharAdvance(uint32_t c) const;
		size_t getStringWidth(const std::string &s) const;
		void printString(const std::string &s, float x, float y, float scale, Align align);
};

#endif

