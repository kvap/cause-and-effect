/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <string>
#include <sstream>

#include "../util/convert.hpp"
#include "../util/Logger.hpp"
#include "Fonts.hpp"
#include "Textures.hpp"
#include "../util/UTF8.hpp"

Font *Fonts::genFont(const std::string filename, size_t size) {
	Font *r = new Font(filename, size);
	return r;
}

Font::Font(const std::string filename, size_t size) {
	// These are the characters that get stored to texture.
	// Margins around characters to prevent them from 'bleeding' into each other.
	const size_t margin = 1;
	size_t image_height = 0, image_width = 512;

	// Initialize FreeType
	FT_Library library;
	if (FT_Init_FreeType(&library)) {
		LOG_FATAL("FreeType initializing failed");
	} else {
		LOG_STRING("FreeType initialized");
	}

	// Load the font
	LOG_STRING("Loading font '" + filename + "'");

	FT_Face face;
	int error = FT_New_Face(library, filename.c_str(), 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		LOG_FATAL("Font loading failed: Unknown format of a file.");
	} else if (error) {
		LOG_FATAL("Font loading failed: The file could not be opened or read, or is broken, or else.");
	}
	LOG_STRING("Font loaded");

	// Some checks
	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE)) {
		LOG_FATAL("The font is not scalable)");
	}
	if (!(face->face_flags & FT_FACE_FLAG_HORIZONTAL)) {
		LOG_FATAL("The font is not horizontal)");
	}

	// Set the font size
	FT_Set_Pixel_Sizes(face, size, 0);
	pixel_height = size;

	int max_height = 0;
	size_t x = margin, y = margin;
	// Rasterize all the characters

	int total_chars = 0;
	uint32_t gindex; // glyph index
	uint32_t unicode = FT_Get_First_Char(face, &gindex);

	while (gindex != 0) {
		char buf[100];
		FT_Get_Glyph_Name(face, gindex, buf, 100);
		LOG_STRING("Available character: " + stringify<uint32_t>(unicode) + " (" + buf + ")");
		// Look up the character in the font file.

		if (glyphs.find(gindex) != glyphs.end()) {
			unicode2glyph[unicode] = glyphs[gindex];
			continue;
		}

		// Render the current glyph.
		FT_Load_Glyph(face, gindex, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);

		// Save character metrics
		Glyph *g = glyphs[gindex] = unicode2glyph[unicode] = new Glyph; // shortcut
		g->advance = face->glyph->metrics.horiAdvance / 64;
		g->width = face->glyph->bitmap.width;
		g->height = face->glyph->bitmap.rows;
		g->bearingX = face->glyph->metrics.horiBearingX / 64;
		g->bearingY = face->glyph->metrics.horiBearingY / 64;

		// Compute kerning
		//for (int j = 0; j < 256; j++) {
		//	// Look up the second character in the font file.
		//	int char2_index = FT_Get_Char_Index(face, j);
		//	FT_Vector akerning;
		//	FT_Get_Kerning(face, char_index, char2_index, FT_KERNING_DEFAULT, &akerning);
		//	kerning[i][j] = static_cast<float>(akerning.x) / 64;
		//}

		// Save glyph bitmap
		g->data = new unsigned char[g->width * g->height];
		memcpy(g->data, face->glyph->bitmap.buffer, g->width * g->height);

		// If the line is full, go to the next line
		if (g->width + margin > image_width - x) {
			x = margin;
			y += max_height + margin;
			max_height = 0;
		}
		g->x = x;
		g->y = y;
		max_height = std::max(g->height, max_height);
		x += g->width + margin;

		total_chars += 1;
		unicode = FT_Get_Next_Char(face, unicode, &gindex);
	}
	LOG_STRING("Total chars in atlas: " + stringify<int>(total_chars));
	FT_Done_FreeType(library);

	LOG_STRING("Generating font texture atlas");
	// Compute how high the texture has to be.
	int needed_image_height = y + max_height + margin;
	// Get the first power of two in which it fits.
	image_height = 16;
	while (image_height < needed_image_height) {
		image_height *= 2;
	}

	// Allocate memory for the texture, and set it to 0.
	unsigned char *image = new unsigned char[image_height * image_width];
	for (int i = 0; i < image_height * image_width; i++) {
		image[i] = 0;
	}

	// Drawing loop
	for (std::map<uint32_t, Glyph*>::iterator gindex = glyphs.begin(); gindex != glyphs.end(); gindex++) {
		Glyph *g = gindex->second; // shortcut

		if (g->data != NULL) {
			// Fill in the texture coords
			g->tex_left = static_cast<float>(g->x) / image_width;
			g->tex_right = static_cast<float>(g->x + g->width) / image_width;
			g->tex_top = static_cast<float>(g->y) / image_height;
			g->tex_bottom = static_cast<float>(g->y + g->height) / image_height;

			// Copy the glyph bitmap to the texture atlas
			for (size_t row = 0; row < g->height; ++row) {
				memcpy(&image[g->x + (g->y + row) * image_width], &g->data[row * g->width], g->width);
			}

			delete[] g->data;
			g->data = NULL;
		}
	}
	LOG_STRING("Font texture atlas generated");

	// Create OpenGL texture from the image.
	glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, image_width, image_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, image);
	// Free the image memory.
	delete[] image;
	LOG_STRING("Font texture loaded");
}

void Font::printString(const std::string &s, float x, float y, float scale, Align align) {
	if (s.empty()) {
		return;
	}

	std::istringstream manystr;
	manystr.str(s);

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(scale, scale, 1);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	std::string onestr;
	float cur_y = 0;
	while (getline(manystr, onestr)) {
		float cur_x = 0;
		switch (align) {
			case ALIGN_CENTER:
				cur_x -= getStringWidth(onestr) / 2;
				break;
			case ALIGN_RIGHT:
				cur_x -= getStringWidth(onestr);
				break;
			default:
				break;
		}
		// Print one string
		std::string::const_iterator i = onestr.begin();
		uint32_t prev = 0;
		while (i != onestr.end()) {
			uint32_t c = get_unicode_char(i);
			if (unicode2glyph.find(c) != unicode2glyph.end()) {
				Glyph *g = unicode2glyph.find(c)->second; // shortcut
				//if (kerning.find(std::make_pair(prev, c)) != kerning.end()) {
				//	cur_x += kerning[std::make_pair(prev, c)];
				//}
				float left = cur_x + g->bearingX;
				float right = left + g->width;
				float top = cur_y + g->bearingY;
				float bottom = top - g->height;
				glTexCoord2f(g->tex_left, g->tex_top);
				glVertex2f(left, top);
				glTexCoord2f(g->tex_right, g->tex_top);
				glVertex2f(right, top);
				glTexCoord2f(g->tex_right, g->tex_bottom);
				glVertex2f(right, bottom);
				glTexCoord2f(g->tex_left, g->tex_bottom);
				glVertex2f(left, bottom);
				cur_x += g->advance;
				prev = c;
			}
			//i++;
		}

		// Advance down
		cur_y -= getLineHeight();
	}
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
}

size_t Font::getLineHeight() const {
	return pixel_height;
}

size_t Font::getCharAdvance(uint32_t c) const {
	if (glyphs.find(c) != glyphs.end()) {
		return glyphs.find(c)->second->advance;
	} else {
		return 0;
	}
}

size_t Font::getStringWidth(const std::string &s) const {
	float total = 0;
	std::string::const_iterator i = s.begin();
	uint32_t prev = 0;
	while (i != s.end()) {
		uint32_t c = get_unicode_char(i);
		if (unicode2glyph.find(c) != unicode2glyph.end()) {
			total += unicode2glyph.find(c)->second->advance;
			//if (kerning.find(std::make_pair(prev, c)) != kerning.end()) {
			//	total += kerning[make_pair(prev, c)];
			//}
			prev = c;
		}
		//i++;
	}
	return static_cast<size_t>(total);
}

Font::~Font() {
	// Delete Glyph structures from memory
	std::map<unsigned int, Glyph*>::iterator i = glyphs.begin();
	while (i != glyphs.end()) {
		if (i->second != NULL) {
			delete i->second;
		}
		i++;
	}
}
