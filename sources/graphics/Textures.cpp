/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <png.h>
#include <malloc.h>

#include "../util/convert.hpp"
#include "../util/Logger.hpp"
#include "Textures.hpp"

typedef std::map<std::string, GLuint> TexturesMap;
TexturesMap textures;
TexturesMap widths;
TexturesMap heights;

void initTextures() {
}

unsigned int loadTexture(std::string filename) {
	return loadTexture(filename, NULL, NULL);
}

unsigned int loadTexture(std::string filename, int *wid, int *hei) {
	unsigned int result;

	// check if this file is already loaded
	TexturesMap::iterator iter = textures.find(filename);
	if (iter != textures.end()) {
		// such texture is already loaded
		result = iter->second;
		if (wid) {
			*wid = widths[filename];
		}
		if (hei) {
			*hei = heights[filename];
		}
	} else {
		// this is not loaded yet texture
		LOG_STRING("Loading texture '" + filename + "'");

		FILE *f = fopen(filename.c_str(), "rb");
		if (!f) {
			LOG_ERROR("Cannot open file '" + filename + "'");
			return 0;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr) {
			LOG_ERROR("png_create_read_struct() failed");
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return 0;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			LOG_ERROR("png_create_info_struct() failed");
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return 0;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			LOG_ERROR("Error during reading PNG");
			return 0;
		}

		png_init_io(png_ptr, f);
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);

		unsigned char **image_rows = (unsigned char **)png_get_rows(png_ptr, info_ptr);

		int bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);
		int color_type = png_get_color_type(png_ptr, info_ptr);
		int width = png_get_image_width(png_ptr, info_ptr);
		int height = png_get_image_height(png_ptr, info_ptr);

		widths[filename] = width;
		heights[filename] = height;
		if (wid) {
			*wid = width;
		}
		if (hei) {
			*hei = height;
		}

		unsigned char *image_data = (unsigned char *)malloc(bytes_per_row * height);
		for (int i = 0; i < height; i++) {
			memcpy(image_data + i * bytes_per_row, image_rows[height - i - 1], bytes_per_row);
		}

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		float maxAniso = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		LOG_STRING("OpenGL max anisotropy: " + stringify<float>(maxAniso));
		if (maxAniso > 4) {
			maxAniso = 4;
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		switch (color_type) {
			case PNG_COLOR_TYPE_RGB:
				LOG_STRING("PNG color type = RGB");
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
				//glGenerateMipmap(GL_TEXTURE_2D);
				break;
			case PNG_COLOR_TYPE_RGBA:
				LOG_STRING("PNG color type = RGBA");
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				//glGenerateMipmap(GL_TEXTURE_2D);
				break;
			default:
				LOG_ERROR("Unsupported color type in the PNG");
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(image_data);
		textures[filename] = result;
		fclose(f);
	}
	// */
	return result;
}

void unloadTexture(std::string filename) {
	TexturesMap::iterator iter = textures.find(filename);
	if (iter != textures.end() ) {
		// such texture is loaded
		glDeleteTextures(1,&(iter->second));
		textures.erase(iter);
	}
}

void wipeTextures() {
	while (!textures.empty()) {
		glDeleteTextures(1,&(textures.begin()->second));
		textures.erase(textures.begin());
	}
}
